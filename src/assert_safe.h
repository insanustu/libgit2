/*
 * Copyright (C) the libgit2 contributors. All rights reserved.
 *
 * This file is part of libgit2, distributed under the GNU GPL v2 with
 * a Linking Exception. For full terms see the included COPYING file.
 */
#ifndef INCLUDE_assert_safe_h__
#define INCLUDE_assert_safe_h__

/*
 * In a debug build, we'll assert(3) for aide in debugging.  In release builds,
 * we will provide macros that will set an error message that indicate a failure
 * and return.  Note that memory leaks can occur in a release-mode assertion
 * failure -- it is impractical to provide safe clean up routines in these very
 * extreme failures, but care should be taken to not leak very large objects.
 */

#if (defined(_DEBUG) || defined(GIT_ASSERT_HARD)) && GIT_ASSERT_HARD != 0
# define GIT_ASSERT(expr) assert(expr)
# define GIT_ASSERT_ARG(expr) assert(expr)

# define GIT_ASSERT_EXT(expr, fail) assert(expr)
# define GIT_ASSERT_ARG_EXT(expr, fail) assert(expr)
#else

/* Internal consistency check to stop the function. */
# define GIT_ASSERT(expr) GIT_ASSERT_EXT(expr, return -1);

/**
 * Assert that a consumer-provided argument is valid, setting an
 * actionable error message and returning -1 if it is not.
 */
# define GIT_ASSERT_ARG(expr) GIT_ASSERT_ARG_EXT(expr, return -1);

/* Internal consistency check to invoke the `fail` param on failure. */
# define GIT_ASSERT_EXT(expr, fail) \
	GIT_ASSERT__EXT(expr, GIT_ERROR_INTERNAL, "unrecoverable internal error", fail)

/*
 * Assert that a consumer-provided argument is valid, setting an
 * actionable error message and running the `fail` param if not.
 */
# define GIT_ASSERT_ARG_EXT(expr, fail) \
	GIT_ASSERT__EXT(expr, GIT_ERROR_INVALID, "invalid argument", fail)

# define GIT_ASSERT__EXT(expr, code, msg, fail) do { \
		if (!(expr)) { \
			git_error_set(code, "%s: '%s'", msg, #expr); \
			fail; \
		} \
	} while(0)

#endif /* GIT_ASSERT_HARD */

#endif
