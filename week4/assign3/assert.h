#ifndef ASSERT_H
#define ASSERT_H

void abort(void);
#define assert(x) if(!(x)) { abort(); }
#define invalid_code_path assert(!"invalid code path")

#endif