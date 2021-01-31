#ifndef INCG_CL_USE_UNBUFFERED_IO_HPP
#define INCG_CL_USE_UNBUFFERED_IO_HPP

namespace cl {
/*!
 * \brief Routine to activate unbuffered I/O.
 * \note This is only really useful for debugging purposes as the output to
 *       output streams that would normally be buffered (e.g., stdout) are
 *       unbuffered so that the output written to those output streams shows up
 *       even if the application crashes rather than leaving some output in
 *       those buffers.
 **/
void useUnbufferedIo();
} // namespace cl
#endif // INCG_CL_USE_UNBUFFERED_IO_HPP
