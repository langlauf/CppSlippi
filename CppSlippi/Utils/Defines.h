#if defined(_WIN32)
	#define CPP_SLIPPI_CLASS_API __pragma(warning(suppress: 4275))
	#if defined(CPP_SLIPPI_EXPORT)
		#define CPP_SLIPPI_API __declspec(dllexport)
	#elif defined(CPP_SLIPPI_SHARED)
		#define CPP_SLIPPI_API __declspec(dllimport)
	#else
		#define CPP_SLIPPI_API
	#endif
#else
	#define CPP_SLIPPI_CLASS_API
	#if defined(CPP_SLIPPI_EXPORT) || defined(CPP_SLIPPI_SHARED)
		#if defined(__GNUC__) || defined(__clang__)
			#define CPP_SLIPPI_API __attribute__((visibility("default")))
		#else
			#define CPP_SLIPPI_API
		#endif
	#endif
#endif
