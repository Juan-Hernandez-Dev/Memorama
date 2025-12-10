#pragma once
#include <string>
#include <vector>

struct Pareja {
    std::string pregunta;
    std::string respuesta;
    std::string categoria;
};

class ReglasPareja {
public:
    virtual ~ReglasPareja() = default;
    virtual bool esPareja(const std::string& id1, const std::string& id2) const = 0;
    virtual std::vector<Pareja> cargarParejas(const std::string& nombreArchivo) = 0;
    virtual std::string obtenerCategoria() const = 0;
};

class ReglasMatematicas : public ReglasPareja {
public:
    bool esPareja(const std::string& id1, const std::string& id2) const override;
    std::vector<Pareja> cargarParejas(const std::string& nombreArchivo) override;
    std::string obtenerCategoria() const override { return "MATEMATICAS"; }
};

class ReglasGeografia : public ReglasPareja {
public:
    bool esPareja(const std::string& id1, const std::string& id2) const override;
    std::vector<Pareja> cargarParejas(const std::string& nombreArchivo) override;
    std::string obtenerCategoria() const override { return "GEOGRAFIA"; }
};

class ReglasVocabulario : public ReglasPareja {
public:
    bool esPareja(const std::string& id1, const std::string& id2) const override;
    std::vector<Pareja> cargarParejas(const std::string& nombreArchivo) override;
    std::string obtenerCategoria() const override { return "VOCABULARIO"; }
};

class ReglasCiencias : public ReglasPareja {
public:
    bool esPareja(const std::string& id1, const std::string& id2) const override;
    std::vector<Pareja> cargarParejas(const std::string& nombreArchivo) override;
    std::string obtenerCategoria() const override { return "CIENCIAS"; }
};
