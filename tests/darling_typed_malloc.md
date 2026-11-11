# Typed allocation compatibility

Compile darling_typed_malloc.c for Darling and run against libSystem. The test resolves the three entry points dynamically, so an older runtime fails explicitly. It checks calloc zeroing/overflow, realloc data preservation and failed-realloc ownership, errno, NULL input and zero-size allocation/free interoperability. No pointer-authentication override is needed.

The test also exercises `malloc_type_zone_malloc_with_options` and its `_internal` alias: the CLEAR option zeroes a plain and an aligned block, an alignment above the default is honoured, the canonical-tag option is accepted, and a non-power-of-two alignment, a size that is not a multiple of the alignment, or an unknown option bit fails with EINVAL.

Signatures follow Apple libmalloc include/malloc/malloc.h (the 5-argument form is `(zone, alignment, size, type_id, opts)` with `MALLOC_ZONE_MALLOC_DEFAULT_ALIGN` as the pointer size). Type identifiers are accepted but not used for type-based heap segregation; these entry points preserve Darling existing allocator behavior. No malloc_zone_t ABI change is made. Additional typed APIs are outside this patch.

Reference: https://github.com/apple-oss-distributions/libmalloc/blob/main/include/malloc/_malloc_type.h
