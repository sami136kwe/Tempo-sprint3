#include "json_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "validation.h"
#include "datetime.h"
#include "timeseries.h"


/**
 * Reports a JSON parse error and exits
 */
void report_json_parse_error(const char* message) {
  fprintf(stderr, "error: JSON parse error (%s)\n", message);
  exit(2);
}

/**
 * Reports a missing field in the JSON and exits
 */ 
void report_json_missing_field(const char* field) {
  fprintf(stderr, "error: missing required field '%s'\n", field);
  exit(2);
}

int json_validate_structure(json_t* root) {
  if (!json_is_object(root)) {
    return 0;
  }
  
  json_t* origin = json_object_get(root, "origin");
  if (!origin || !json_is_string(origin)) {
    return 0;
  }
  
  json_t* observations = json_object_get(root, "observations");
  if (!observations || !json_is_array(observations)) {
    return 0;
  }
  
  return 1;
}

/**
 * Parses the "origin" field from the JSON and sets the start datetime
 *
 * @param root        The root JSON object
 * @param timeseries  The timeseries to update
*/
void json_parse_origin(json_t* root, struct Timeseries* timeseries) {
  json_t* origin = json_object_get(root, "origin");
  if (!origin) {
    report_json_missing_field("origin");
  }
  
  const char* origin_str = json_string_value(origin);
  int y, m, d, h, min, s;
  
  if (sscanf(origin_str, "%4d-%2d-%2dT%2d:%2d:%2d", 
             &y, &m, &d, &h, &min, &s) == 6) {
    datetime_initialize(&timeseries->start_datetime, y, m, d, h, min, s);
  } else {
    report_invalid_datetime_format();
  }
}

/**
 * Parses the "observations" array from the JSON and adds observations
 *
 * @param root        The root JSON object
 * @param timeseries  The timeseries to update
*/
void json_parse_observations(json_t* root, struct Timeseries* timeseries) {
  json_t* observations = json_object_get(root, "observations");
  if (!observations) {
    report_json_missing_field("observations");
  }
  
  size_t index;
  json_t* obs;
  
  json_array_foreach(observations, index, obs) {
    if (!json_is_object(obs)) {
      report_json_parse_error("observation must be an object");
    }
    
    json_t* offset_json = json_object_get(obs, "offset");
    json_t* value_json = json_object_get(obs, "value");
    
    if (!offset_json || !json_is_integer(offset_json)) {
      report_json_parse_error("invalid or missing offset");
    }
    
    if (!value_json || !json_is_integer(value_json)) {
      report_json_parse_error("invalid or missing value");
    }
    
    json_int_t offset_int = json_integer_value(offset_json);
    json_int_t value_int = json_integer_value(value_json);
    
    if (offset_int < 0) {
      report_json_parse_error("offset must be non-negative");
    }
    
    int offset = (int)offset_int;
    int value = (int)value_int;
    
    timeseries_add_observation(timeseries, offset, value);
  }
}

void json_parse_timeseries_from_stdin(struct Timeseries* timeseries) {
  json_error_t error;
  json_t* root = json_loadf(stdin, 0, &error);
  
  if (!root) {
    fprintf(stderr, "error: JSON parsing failed at line %d: %s\n",
            error.line, error.text);
    exit(2);
  }
  
  if (!json_validate_structure(root)) {
    json_decref(root);
    report_json_parse_error("invalid timeseries structure");
  }
  
  timeseries_initialize(timeseries);
  json_parse_origin(root, timeseries);
  json_parse_observations(root, timeseries);
  timeseries_set_last_datetime(timeseries);
  
  json_decref(root);
}
