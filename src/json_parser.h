#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <jansson.h>
#include "timeseries.h"

/**
 * Parse une série temporelle depuis un flux JSON
 *
 * @param timeseries  La série temporelle à initialiser
 */
void json_parse_timeseries_from_stdin(struct Timeseries* timeseries);

/**
 * Valide la structure d'un objet JSON de série temporelle
 *
 * @param root  L'objet JSON racine
 * @return      1 si valide, 0 sinon
 */
int json_validate_structure(json_t* root);

#endif
