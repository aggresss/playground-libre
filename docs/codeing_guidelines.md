
## Coding guidelines

* Use enum for constants where appropriate
* Use const as much as possible (where appropriate)
* Use C99 data types (intN_t, uintN_t, bool)
* Hide data-types in .c files where possible (use struct foo)
* Avoid malloc/free, use mem_alloc/mem_deref instead
* CVS/svn/git tags are NOT allowed in the code!
* Avoid bit-fields in structs which are not portable
* Use dummy handlers for timing-critical callbacks
* return err, return alloced objects as pointer-pointers
* in allocating functions, first arg is always double pointer
* Use POSIX error-codes; EINVAL for invalid args, EBADMSG for
  parse errors and EPROTO for protocol errors

