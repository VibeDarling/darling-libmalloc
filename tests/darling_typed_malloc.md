# Typed allocation compatibility

Compile darling_typed_malloc.c for Darling and run against libSystem. The test resolves the three entry points dynamically, so an older runtime fails explicitly. It checks calloc zeroing/overflow, realloc data preservation and failed-realloc ownership, errno, NULL input and zero-size allocation/free interoperability. No pointer-authentication override is needed.

Signatures follow Apple libmalloc include/malloc/_malloc_type.h. Type identifiers are accepted but not used for type-based heap segregation; these entry points preserve Darling existing allocator behavior. No malloc_zone_t ABI change is made. Additional typed APIs are outside this patch.

Reference: https://github.com/apple-oss-distributions/libmalloc/blob/main/include/malloc/_malloc_type.h
