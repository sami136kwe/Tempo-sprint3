#include <tap.h>

#include "datetime.h"

int main(void) {
  struct Datetime dt1;
  datetime_initialize(&dt1, 2025, 11, 1, 9, 0, 0);
  struct Datetime dt2 = datetime_copy(&dt1);

  cmp_ok(datetime_compare(&dt1, &dt2), "==", 0,
         "The copy of a datetime is equal to the source datetime");

  datetime_delete(&dt1);
  datetime_delete(&dt2);
  done_testing();
}
