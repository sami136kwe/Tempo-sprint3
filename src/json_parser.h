#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <jansson.h>
#include "timeseries.h"

/**
 * Parse a time series from a JSON stream
 *
 * @param timeseries  The time series to initializer
 */
void json_parse_timeseries_from_stdin(struct Timeseries* timeseries);

/**
 * Validate the structure of a JSON time series object
 *
 * @param root  The root JSON object
 * @return      1 if valid, 0 otherwise
 */
int json_validate_structure(json_t* root);

#endif
