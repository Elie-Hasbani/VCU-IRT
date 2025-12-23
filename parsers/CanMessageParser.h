#ifndef CAN_MESSAGE_PARSER_H
#define CAN_MESSAGE_PARSER_H

#include <string>
#include <fstream>

/**
 * @brief Message issu du fichier messages.csv
 * Format attendu : idMessage;value
 */
struct Message
{
    int id;
    int value;
};

/**
 * @brief Parser CSV pour messages CAN (VCU)
 *
 * - Ignore lignes vides
 * - Ignore commentaires (# ...)
 * - Ignore sauts de ligne
 * - Parse uniquement les lignes valides id;value
 */
class CanMessageParser
{
public:
    /**
     * @brief Constructeur
     * @param filename Chemin du fichier CSV
     */
    explicit CanMessageParser(const std::string &filename);

    /**
     * @brief Destructeur
     */
    ~CanMessageParser();

    /**
     * @brief Lit le prochain message valide
     * @param msg Message parsé (sortie)
     * @return true si un message valide est lu, false si EOF
     */
    bool readNext(Message &msg);

    /**
     * @brief Remet la lecture au début du fichier
     */
    void reset();

private:
    std::ifstream file;
    std::string filename;

    static void trim(std::string &s);
};

#endif // CAN_MESSAGE_PARSER_H
