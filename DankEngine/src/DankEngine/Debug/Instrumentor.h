#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <iomanip>

#include <thread>


namespace Dank
{
    using FloatingPointMicroSeconds = std::chrono::duration<double, std::micro>;

    struct ProfileResult
    {
        std::string Name;

        FloatingPointMicroSeconds Start;
        std::chrono::microseconds ElapsedTime;
        std::thread::id ThreadID;
    };

    struct InstrumentationSession
    {
        std::string Name;
    };

    class Instrumentor
    {
    private:
        InstrumentationSession* _currentSession;
        std::ofstream _outputStream;
        std::mutex _mutex;
    public:
        Instrumentor()
            : _currentSession(nullptr)
        {
        }

        void BeginSession(const std::string& name, const std::string& filepath = "results.json")
        {
            std::lock_guard lock(_mutex);

            // If there's already a current session, close it before beginning a new one.
            // BeginSession() might occur before Log::Init(), send error if so and end session
            if (_currentSession)
            {
                if (Log::GetCoreLogger())                
                    DANK_CORE_ERROR("Instrumentor::BeginSession('{0}') when sesion '{1}' already open", name, _currentSession->Name);                

                InternalEndSession();                
            }

            _outputStream.open(filepath);
            
            if (_outputStream.is_open())
            {
                _currentSession = new InstrumentationSession({ name });
                WriteHeader();
            }
            else
            {
                if (Log::GetCoreLogger())
                {
                    DANK_CORE_ERROR("Instrumentor could not open results file '{0}'", filepath);
                }
            }
        }

        void EndSession()
        {
            std::lock_guard lock(_mutex);
            InternalEndSession();
        }

        void WriteProfile(const ProfileResult& result)
        {
            std::stringstream json;

            std::string name = result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');

            json << std::setprecision(3) << std::fixed;
            json << ",{";
            json << "\"cat\":\"function\",";
            json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
            json << "\"name\":\"" << result.Name << "\",";
            json << "\"ph\":\"X\",";
            json << "\"pid\":0,";
            json << "\"tid\":" << result.ThreadID << ",";
            json << "\"ts\":" << result.Start.count();
            json << "}";

            std::lock_guard lock(_mutex);
            if (_currentSession)
            {
                _outputStream << json.str();
                _outputStream.flush();
            }
        }

        void WriteHeader()
        {
            _outputStream << "{\"otherData\": {},\"traceEvents\":[{}";
            _outputStream.flush();
        }

        void WriteFooter()
        {
            _outputStream << "]}";
            _outputStream.flush();
        }

        static Instrumentor& Get()
        {
            static Instrumentor instance;
            return instance;
        }

    private:
        // Must own a lock on _mutex before calling
        void InternalEndSession()
        {
            if (_currentSession)
            {
                WriteFooter();
                _outputStream.close();
                delete _currentSession;
                _currentSession = nullptr;
            }
        }
    };

    class InstrumentationTimer
    {
    public:
        InstrumentationTimer(const char* name)
            : _name(name), _stopped(false)
        {
            _startTimepoint = std::chrono::high_resolution_clock::now();
        }

        ~InstrumentationTimer()
        {
            if (!_stopped)
                Stop();
        }

        void Stop()
        {
            auto endTimepoint = std::chrono::steady_clock::now();
            auto highResStart = FloatingPointMicroSeconds{ _startTimepoint.time_since_epoch() };
            auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(_startTimepoint).time_since_epoch();

            Instrumentor::Get().WriteProfile({_name, highResStart, elapsedTime, std::this_thread::get_id() });

            _stopped = true;
        }
    private:
        const char* _name;
        std::chrono::time_point<std::chrono::steady_clock> _startTimepoint;
        bool _stopped;
    };

    namespace InstrumentorUtils {

        template <size_t N>
        struct ChangeResult
        {
            char Data[N];
        };

        template <size_t N, size_t K>
        constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
        {
            ChangeResult<N> result = {};

            size_t srcIndex = 0;
            size_t dstIndex = 0;
            while (srcIndex < N)
            {
                size_t matchIndex = 0;
                while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
                    matchIndex++;
                if (matchIndex == K - 1)
                    srcIndex += matchIndex;
                result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
                srcIndex++;
            }
            return result;
        }
    }
}

#define DANK_PROFILE 0
#if DANK_PROFILE
    #if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
    #define DANK_FUNC_SIG __PRETTY_FUNCTION__
    #elif defined(__DMC__) && (__DMC__ >= 0x810)
    #define DANK_FUNC_SIG __PRETTY_FUNCTION__
    #elif (defined(__FUNCSIG__) || (_MSC_VER))
    #define DANK_FUNC_SIG __FUNCSIG__
    #elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
    #define DANK_FUNC_SIG __FUNCTION__
    #elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
    #define DANK_FUNC_SIG __FUNC__
    #elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
    #define DANK_FUNC_SIG __func__
    #elif defined(__cplusplus) && (__cplusplus >= 201103)
    #define DANK_FUNC_SIG __func__
    #else
    #define DANK_FUNC_SIG "DANK_FUNC_SIG unknown!"
    #endif

    #define DANK_PROFILE_BEGIN_SESSION(name, filepath) ::Dank::Instrumentor::Get().BeginSession(name, filepath)
    #define DANK_PROFILE_END_SESSION() ::Dank::Instrumentor::Get().EndSession()
    #define DANK_PROFILE_SCOPE(name) constexpr auto fixedName = ::Dank::InstrumentorUtils::CleanupOutputString(name, "__cdecl ");\
									    ::Dank::InstrumentationTimer timer##__LINE__(fixedName.Data)
    #define DANK_PROFILE_FUNCTION() DANK_PROFILE_SCOPE(DANK_FUNC_SIG)
#else
    #define DANK_PROFILE_BEGIN_SESSION(name, filepath)
    #define DANK_PROFILE_END_SESSION()
    #define DANK_PROFILE_SCOPE(name)
#define DANK_PROFILE_FUNCTION()
#endif