#ifndef KE_ERR_H
#define KE_ERR_H

enum ErrorCode {
	NONE,
	MEM_ALLOC,
	OPEN_FILE,
	EXT_MISMATCH,
	NO_CONTENT,
	PARSE_FAILED,
	ALREADY_INITED
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

#endif // KE_ERR_H
