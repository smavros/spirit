#include <engine/Method.hpp>
#include <engine/Vectormath.hpp>
#include <engine/Manifoldmath.hpp>
#include <utility/Logging.hpp>
#include <utility/Timing.hpp>
#include <utility/Exception.hpp>
#include <utility/Constants.hpp>

#include <sstream>
#include <iomanip>

using namespace Utility;

namespace Engine
{
    Method::Method(std::shared_ptr<Data::Parameters_Method> parameters, int idx_img, int idx_chain) :
        parameters(parameters), idx_image(idx_img), idx_chain(idx_chain)
    {
        // Sender name for log messages
        this->SenderName = Utility::Log_Sender::All;

        // Default history contains force_max_abs_component
        this->history = std::map<std::string, std::vector<scalar>>{
            {"force_max_abs_component", {this->force_max_abs_component}} };

        // TODO: is this a good idea?
        this->n_iterations     = this->parameters->n_iterations;
        this->n_iterations_log = this->parameters->n_iterations_log;
		if (this->n_iterations_log > 0)
			this->n_log        = this->n_iterations / this->n_iterations_log;
		else
			this->n_log        = 0;

        // Setup timings
        for (int i = 0; i<7; ++i) this->t_iterations.push_back(system_clock::now());
        this->ips = 0;
        this->starttime = Timing::CurrentDateTime();


        // Printing precision for scalars
        #ifdef CORE_SCALAR_TYPE_FLOAT
            this->print_precision = 8;
        #else
            this->print_precision = 12;
        #endif
    }


    void Method::Iterate()
    {
        //---- Initialise Solver-specific variables
        this->Initialize();

        //---- Start timings
        this->starttime = Timing::CurrentDateTime();
        this->t_start = system_clock::now();
        auto t_current = system_clock::now();
        this->t_last = system_clock::now();

        //---- Log messages
        this->Message_Start();

        //---- Initial save
        this->Save_Current(this->starttime, this->iteration, true, false);

        //---- Iteration loop
        this->iteration = 0;
        this->step      = 0;
        for ( this->iteration = 0; 
              this->ContinueIterating() &&
              !this->Walltime_Expired(t_current - t_start); 
              ++this->iteration )
        {
            t_current = system_clock::now();

            // Lock Systems
            this->Lock();

            // Pre-iteration hook
            this->Hook_Pre_Iteration();
            // Do one single Iteration
            this->Iteration();
            // Post-iteration hook
            this->Hook_Post_Iteration();

            // Recalculate FPS
            this->t_iterations.pop_front();
            this->t_iterations.push_back(system_clock::now());

            // Log Output every n_iterations_log steps
			bool log = false;
			if (this->n_iterations_log > 0)
				log = this->iteration > 0 && 0 == fmod(this->iteration, this->n_iterations_log);
            if ( log )
            {
                ++step;
                this->Message_Step();
                this->Save_Current(this->starttime, this->iteration, false, false);
            }

            // Unlock systems
            this->Unlock();
        }

        //---- Log messages
        this->Message_End();

        //---- Final save
        this->Save_Current(this->starttime, this->iteration, false, true);
        //---- Finalize (set iterations_allowed to false etc.)
        this->Finalize();
    }



    scalar Method::getIterationsPerSecond()
    {
        scalar l_ips = 0.0;
        for (unsigned int i = 0; i < t_iterations.size() - 1; ++i)
        {
            l_ips += Timing::SecondsPassed(t_iterations[i+1] - t_iterations[i]);
        }
        this->ips = 1.0 / (l_ips / (t_iterations.size() - 1));
        return this->ips;
    }


    scalar Method::getForceMaxAbsComponent()
    {
        return this->force_max_abs_component;
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////// Protected functions

    void Method::Initialize()
    {

    }

    void Method::Message_Start()
    {

    }
    void Method::Message_Step()
    {

    }
    void Method::Message_End()
    {

    }

    void Method::Iteration()
    {

    }




    bool Method::ContinueIterating()
    {
        return  this->iteration < this->n_iterations &&
                this->Iterations_Allowed() &&
               !this->StopFile_Present()   && 
               !this->Converged();
    }

    bool Method::Converged()
    {
        return false;
    }

    bool Method::Iterations_Allowed()
    {
        return this->systems[0]->iteration_allowed;
    }


    bool Method::Walltime_Expired(duration<scalar> dt_seconds)
    {
        if (this->parameters->max_walltime_sec <= 0)
            return false;
        else
            return dt_seconds.count() > this->parameters->max_walltime_sec;
    }

    bool Method::StopFile_Present()
    {
        std::ifstream f("STOP");
        return f.good();
    }





    void Method::Save_Current(std::string starttime, int iteration, bool initial, bool final)
    {
        // Not Implemented!
        Log(Utility::Log_Level::Error, Utility::Log_Sender::All, std::string("Tried to use Method::Save_Current() of the Method base class!"));
        throw Utility::Exception::Not_Implemented;
    }

    void Method::Hook_Pre_Iteration()
    {
        // Not Implemented!
        Log(Utility::Log_Level::Error, Utility::Log_Sender::All, std::string("Tried to use Method::Hook_Pre_Iteration() of the Method base class!"));
        throw Utility::Exception::Not_Implemented;
    }

    void Method::Hook_Post_Iteration()
    {
        // Not Implemented!
        Log(Utility::Log_Level::Error, Utility::Log_Sender::All, std::string("Tried to use Method::Hook_Post_Iteration() of the Method base class!"));
        throw Utility::Exception::Not_Implemented;
    }

    void Method::Finalize()
    {
        // Not Implemented!
        Log(Utility::Log_Level::Error, Utility::Log_Sender::All, std::string("Tried to use Method::Finalize() of the Method base class!"));
        throw Utility::Exception::Not_Implemented;
    }






    void Method::Lock()
    {
        for (auto& system : this->systems) system->Lock();
    }

    void Method::Unlock()
    {
        for (auto& system : this->systems) system->Unlock();
    }


    std::string Method::Name()
    {
        // Not Implemented!
        Log(Utility::Log_Level::Error, Utility::Log_Sender::All, std::string("Tried to use Method::Name() of the Method base class!"));
        return "--";
    }

    // Solver name as string
    std::string Method::SolverName()
    {
        // Not Implemented!
        Log(Log_Level::Error, Log_Sender::All, std::string("Tried to use Method::SolverName() of the Method base class!"), this->idx_image, this->idx_chain);
        return "--";
    }

    std::string Method::SolverFullName()
    {
        // Not Implemented!
        Log(Log_Level::Error, Log_Sender::All, std::string("Tried to use Method::SolverFullname() of the Method base class!"), this->idx_image, this->idx_chain);
        return "--";
    }
}