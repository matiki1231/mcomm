#ifndef PROTOCOL_H
#define PROTOCOL_H

#define PASSWORD_LENGTH 32
#define NAME_LENGTH 32
#define MESSAGE_LENGTH 256

typedef struct {
  char value[NAME_LENGTH];
} name_t;

typedef enum {
  REQUEST_PASSWORD,
  RESPONSE_PASSWORD,
  REQUEST_NAME,
  RESPONSE_NAME,
  REPONSE_NAMES,
  REQUEST_MESSAGE,
  RESPONSE_MESSAGE
} packettype_t;

typedef struct {
  char pass[PASSWORD_LENGTH];
} resppass_t;

typedef struct {
  name_t name;
} respname_t;

typedef struct {
  char message[MESSAGE_LENGTH];
} reqmessage_t;

typedef struct {
  char message[MESSAGE_LENGTH];
  name_t author;
} respmessage_t;

typedef struct {
  packettype_t type;
  union {
    resppass_t resppass;
    respname_t respname;
    reqmessage_t reqmessage;
    respmessage_t respmessage;
  } data;
} packet_t;

#endif
