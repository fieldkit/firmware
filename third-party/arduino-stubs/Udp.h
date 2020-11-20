#pragma once

#include <cinttypes>

class IPAddress {
private:
    union {
	uint8_t bytes[4];  // IPv4 address
	uint32_t dword;
    } _address;

    // Access the raw byte array containing the address.  Because this returns a pointer
    // to the internal structure rather than a copy of the address this function should only
    // be used when you know that the usage of the returned uint8_t* will be transient and not
    // stored.
    uint8_t* raw_address() { return _address.bytes; };

public:
    // Constructors
    IPAddress();
    IPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    IPAddress(uint32_t address);
    IPAddress(const uint8_t *address);

    // bool fromString(const char *address);
    // bool fromString(const String &address) { return fromString(address.c_str()); }

    // Overloaded cast operator to allow IPAddress objects to be used where a pointer
    // to a four-byte uint8_t array is expected
    operator uint32_t() const { return _address.dword; };
    bool operator==(const IPAddress& addr) const { return _address.dword == addr._address.dword; };
    bool operator==(const uint8_t* addr) const;

    // Overloaded index operator to allow getting and setting individual octets of the address
    uint8_t operator[](int index) const { return _address.bytes[index]; };
    uint8_t& operator[](int index) { return _address.bytes[index]; };

    // Overloaded copy operators to allow initialisation of IPAddress objects from other types
    IPAddress& operator=(const uint8_t *address);
    IPAddress& operator=(uint32_t address);

    // virtual size_t printTo(Print& p) const;

    // friend class EthernetClass;
    friend class UDP;
    // friend class Client;
    // friend class Server;
    // friend class DhcpClass;
    // friend class DNSClient;
};

const IPAddress INADDR_NONE(0, 0, 0, 0);

class UDP {

public:
  virtual uint8_t begin(uint16_t) { return true; }
  virtual uint8_t beginMulticast(IPAddress, uint16_t) { return 0; }  // initialize, start listening on specified multicast IP address and port. Returns 1 if successful, 0 on failure
  virtual void stop() = 0;  // Finish with the UDP socket

  // Sending UDP packets
  // Start building up a packet to send to the remote host specific in ip and port
  // Returns 1 if successful, 0 if there was a problem with the supplied IP address or port
  virtual int beginPacket(IPAddress ip, uint16_t port) = 0;
  // Start building up a packet to send to the remote host specific in host and port
  // Returns 1 if successful, 0 if there was a problem resolving the hostname or port
  virtual int beginPacket(const char *host, uint16_t port) = 0;
  // Finish off this packet and send it
  // Returns 1 if the packet was sent successfully, 0 if there was an error
  virtual int endPacket() = 0;
  // Write a single byte into the packet
  virtual size_t write(uint8_t) = 0;
  // Write size bytes from buffer into the packet
  virtual size_t write(const uint8_t *buffer, size_t size) = 0;

  // Start processing the next available incoming packet
  // Returns the size of the packet in bytes, or 0 if no packets are available
  virtual int parsePacket() = 0;
  // Number of bytes remaining in the current packet
  virtual int available() = 0;
  // Read a single byte from the current packet
  virtual int read() = 0;
  // Read up to len bytes from the current packet and place them into buffer
  // Returns the number of bytes read, or 0 if none are available
  virtual int read(unsigned char* buffer, size_t len) = 0;
  // Read up to len characters from the current packet and place them into buffer
  // Returns the number of characters read, or 0 if none are available
  virtual int read(char* buffer, size_t len) = 0;
  // Return the next byte from the current packet without moving on to the next byte
  virtual int peek() = 0;
  virtual void flush() = 0;	// Finish reading the current packet

  // Return the IP address of the host who sent the current incoming packet
  virtual IPAddress remoteIP() = 0;
  // Return the port of the host who sent the current incoming packet
  virtual uint16_t remotePort() = 0;
protected:
    // uint8_t* rawIPAddress(IPAddress& addr) { return addr.raw_address(); };
};
