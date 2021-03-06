#pragma once
#ifndef IO_DATAWRITER_H
#define IO_DATAWRITER_H

#include <data/Geometry.hpp>
#include <data/Spin_System.hpp>
#include <data/Spin_System_Chain.hpp>

namespace IO
{
    // =========================== Saving Configurations ===========================
    void Save_To_OVF( const vectorfield & vf, const Data::Geometry & geometry, std::string outputfilename );
    // Append Spin_Configuration to file
    void Write_Spin_Configuration(const std::shared_ptr<Data::Spin_System> & s, const int iteration, const std::string fileName, bool append=false);
    // Saves Spin_Chain_Configuration to file
    void Save_SpinChain_Configuration(const std::shared_ptr<Data::Spin_System_Chain> & c, const int iteration, const std::string fileName);

    // =========================== Saving Energies ===========================
    void Write_Energy_Header(const Data::Spin_System & s, const std::string fileName, std::vector<std::string> firstcolumns={"iteration", "E_tot"}, bool contributions=true, bool normalize_nos=true);
    // Appends the Energy of a spin system with energy contributions (without header)
    void Append_System_Energy(const Data::Spin_System &s, const int iteration, const std::string fileName, bool normalize_nos=true);
    // Save energy contributions of a spin system
    void Write_System_Energy(const Data::Spin_System & system, const std::string fileName, bool normalize_by_nos=true);
    // Save energy contributions of a spin system per spin
    void Write_System_Energy_per_Spin(const Data::Spin_System & s, const std::string fileName, bool normalize_nos=true);

    // Saves Energies of all images with header and contributions
    void Write_Chain_Energies(const Data::Spin_System_Chain & c, const int iteration, const std::string fileName, bool normalize_nos=true);
    // Saves the Energies interpolated by the GNEB method
    void Write_Chain_Energies_Interpolated(const Data::Spin_System_Chain & c, const std::string fileName, bool normalize_nos=true);

    // =========================== Saving Forces ===========================
    // Saves the forces on an image chain
    void Write_System_Force(const Data::Spin_System & s, const std::string fileName);
    // Saves the forces on an image chain
    void Write_Chain_Forces(const Data::Spin_System_Chain & c, const std::string fileName);
};
#endif