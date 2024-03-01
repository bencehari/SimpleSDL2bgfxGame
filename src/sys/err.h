#ifndef KE_ERR_H
#define KE_ERR_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Error codes.
*/
enum ErrorCode {
	NONE,				//!< None
	MEM_ALLOC,			//!< Failed allocation memory
	OPEN_FILE,			//!< Failed open file
	EXT_MISMATCH,		//!< File extension problem
	NO_CONTENT,			//!< Expected content doesn't exist (eg: fgets returns NULL)
	PARSE_FAILED,		//!< Parsing failed
	ALREADY_INITED		//!< Reinitialize not permitted
};

/**
 * @brief Prints error message and returns error code.
 *
 * 100 char limit! Make it short!
 * E.g.: whole path -> filename
 *
 * @param _errorCode ErrorCode.
 * @param _format Format like printf first param.
 * @param ... Params like printf second.. param.
 *
 * @return ErrorCode.
*/
extern enum ErrorCode err_create(enum ErrorCode _errorCode, const char* _format, ...);

/**
 * @brief Returns stringified error code.
 *
 * @param _errorCode ErrorCode.
 *
 * @return char*.
*/
extern const char* err_code_str(enum ErrorCode _errorCode);

#ifdef __cplusplus
}
#endif

#endif // KE_ERR_H
