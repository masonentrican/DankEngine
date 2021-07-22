#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>
namespace Dank
{
    struct ProfileResult
    {
        std::string Name;
        long long Start, End;
        uint32_t ThreadID;
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
        int _profileCount;
    public:
        Instrumentor()
            : _currentSession(nullptr), _profileCount(0)
        {
        }

        void BeginSession(const std::string& name, const std::string& filepath = "results.json")
        {
            _outputStream.open(filepath);
            WriteHeader();
            _currentSession = new InstrumentationSession{ name };
        }

        void EndSession()
        {
            WriteFooter();
            _outputStream.close();
            delete _currentSession;
            _currentSession = nullptr;
            _profileCount = 0;
        }

        void WriteProfile(const ProfileResult& result)
        {
            if (_profileCount++ > 0)
                _outputStream << ",";

            std::string name = result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');

            _outputStream << "{";
            _outputStream << "\"cat\":\"function\",";
            _outputStream << "\"dur\":" << (result.End - result.Start) << ',';
            _outputStream << "\"name\":\"" << name << "\",";
            _outputStream << "\"ph\":\"X\",";
            _outputStream << "\"pid\":0,";
            _outputStream << "\"tid\":" << result.ThreadID << ",";
            _outputStream << "\"ts\":" << result.Start;
            _outputStream << "}";

            _outputStream.flush();
        }

        void WriteHeader()
        {
            _outputStream << "{\"otherData\": {},\"traceEvents\":[";
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
            auto endTimepoint = std::chrono::high_resolution_clock::now();

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(_startTimepoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

            uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
            Instrumentor::Get().WriteProfile({ _name, start, end, threadID });

            _stopped = true;
        }
    private:
        const char* _name;
        std::chrono::time_point<std::chrono::high_resolution_clock> _startTimepoint;
        bool _stopped;
    };
}

#define DANK_PROFILE 1

#if DANK_PROFILE
    #define DANK_PROFILE_BEGIN_SESSION(name, filepath) ::Dank::Instrumentor::Get().BeginSession(name, filepath)
    #define DANK_PROFILE_END_SESSION() ::Dank::Instrumentor::Get().EndSession()
    #define DANK_PROFILE_SCOPE(name) ::Dank::InstrumentationTimer timer##__LINE__(name);
    #define DANK_PROFILE_FUNCTION() DANK_PROFILE_SCOPE(__FUNCSIG__)
#else
    #define DANK_PROFILE_BEGIN_SESSION(name, filepath)
    #define DANK_PROFILE_END_SESSION()
    #define DANK_PROFILE_SCOPE(name)
    #define DANK_PROFILE_FUNCTION() 
#endif