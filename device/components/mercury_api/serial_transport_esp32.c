/**
 *  @file serial_transport_esp32.c
 *  @brief Mercury API - ESP32 platform implementation of serial transport
 *  @author Kalin Kochnev
 *  @date 8/13/2024
 *
*/

#include "tm_reader.h"
#include "tmr_serial_transport.h"


/* Stub implementation of serial transport layer routines. */

static TMR_Status
s_open(TMR_SR_SerialTransport *this)
{

  /* This routine should open the serial connection */

  return TMR_ERROR_UNIMPLEMENTED;
}


static TMR_Status
s_sendBytes(TMR_SR_SerialTransport *this, uint32_t length, 
                uint8_t* message, const uint32_t timeoutMs)
{

  /* This routine should send length bytes, pointed to by message on
   * the serial connection. If the transmission does not complete in
   * timeoutMs milliseconds, it should return TMR_ERROR_TIMEOUT.
   */

  return TMR_ERROR_UNIMPLEMENTED;
}


static TMR_Status
s_receiveBytes(TMR_SR_SerialTransport *this, uint32_t length, 
                   uint32_t* messageLength, uint8_t* message, const uint32_t timeoutMs)
{

  /* This routine should receive exactly length bytes on the serial
   * connection and store them into the memory pointed to by
   * message. If the required number of bytes are note received in
   * timeoutMs milliseconds, it should return TMR_ERROR_TIMEOUT.
   */

  return TMR_ERROR_UNIMPLEMENTED;
}


static TMR_Status
s_setBaudRate(TMR_SR_SerialTransport *this, uint32_t rate)
{

  /* This routine should change the baud rate of the serial connection
   * to the specified rate, or return TMR_ERROR_INVALID if the rate is
   * not supported.
   */

  return TMR_ERROR_UNIMPLEMENTED;
}


static TMR_Status
s_shutdown(TMR_SR_SerialTransport *this)
{

  /* This routine should close the serial connection and release any
   * acquired resources.
   */

  return TMR_ERROR_UNIMPLEMENTED;
}

static TMR_Status
s_flush(TMR_SR_SerialTransport *this)
{

  /* This routine should empty any input or output buffers in the
   * communication channel. If there are no such buffers, it may do
   * nothing.
   */

  return TMR_ERROR_UNIMPLEMENTED;
}



/* This function is not part of the API as such. This is for
 * application code to call to fill in the transport object before
 * initializing the reader object itself, as in the following code:
 * 
 * TMR_Reader reader;
 *
 * TMR_SR_SerialTransportDummyInit(&reader.u.serialReader.transport, myArgs);
 * TMR_SR_SerialReader_init(&reader);
 *
 * The initialization should not actually open a communication channel
 * or acquire other communication resources at this time.
 */
TMR_Status
TMR_SR_SerialTransportDummyInit(TMR_SR_SerialTransport *transport,
								TMR_SR_SerialPortNativeContext *context, void *other)
{

  /* Each of the callback functions will be passed the transport
   * pointer, and they can use the "cookie" member of the transport
   * structure to store the information specific to the transport,
   * such as a file handle or the memory address of the FIFO.
   */
  transport->cookie = other;

  transport->open = s_open;
  transport->sendBytes = s_sendBytes;
  transport->receiveBytes = s_receiveBytes;
  transport->setBaudRate = s_setBaudRate;
  transport->shutdown = s_shutdown;
  transport->flush = s_flush;

  return TMR_SUCCESS;
}

/**
 * Initialize a TMR_SR_SerialTransport structure with a given serial device.
 *
 * @param transport The TMR_SR_SerialTransport structure to initialize.
 * @param context A TMR_SR_SerialPortNativeContext structure for the callbacks to use.
 * @param device The path or name of the serial device (@c /dev/ttyS0, @c COM1)
 */
TMR_Status
TMR_SR_SerialTransportNativeInit(TMR_SR_SerialTransport *transport,
                                 TMR_SR_SerialPortNativeContext *context,
                                 const char *device)
{
  return TMR_SUCCESS;
}

