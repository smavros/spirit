#include <Spirit/Log.h>
#include <utility/Logging.hpp>
#include <utility/Exception.hpp>

#include <iostream>
#include <string>

void Log_Send( State *state, int level, int sender, const char * message, int idx_image, int idx_chain )
{
    try
    {
        Log( static_cast<Utility::Log_Level>(level), static_cast<Utility::Log_Sender>(sender), 
             std::string(message), idx_image, idx_chain );
    }
    catch( ... )
    {
        Utility::Handle_Exception( idx_image, idx_chain );
    }
}

std::vector<Utility::LogEntry> Log_Get_Entries(State *state)
{
    try
    {
        // Get all entries
        return Log.GetEntries();
    }
    catch( ... )
    {
        Utility::Handle_Exception();
        
        Utility::LogEntry Error = { std::chrono::system_clock::now(), 
                                    Utility::Log_Sender::API, Utility::Log_Level::Error,
                                    "GetEntries() failed", -1, -1 };
        std::vector<Utility::LogEntry> ret = { Error };
        return ret;
    }
}

void Log_Append(State *state)
{
    try
    {
        Log.Append_to_File();
    }
    catch( ... )
    {
        Utility::Handle_Exception();    
    }
}

void Log_Dump(State *state)
{
    try
    {
        Log.Dump_to_File();
    }
    catch( ... )
    {
        Utility::Handle_Exception();        
    }
}

int Log_Get_N_Entries(State *state)
{
    return Log.n_entries;
}

int Log_Get_N_Errors(State *state)
{
    return Log.n_errors;
}

int Log_Get_N_Warnings(State *state)
{
    return Log.n_warnings;
}