#ifndef MODEL_H
#define MODEL_H

// ======================================================
// PART 1: DEFINES
// ======================================================
#define MAX_ROOM        200
#define MAX_SERVICE_USED 50
#define MAX_MENU        15
#define MAX_INVOICES    500

#define PROMO_CODE      "WELCOME2026"

// Member Levels
#define MEMBER_BRONZE   0
#define MEMBER_SILVER   1
#define MEMBER_GOLD     2

// Discount Rates
#define RATE_SILVER     0.05f
#define RATE_GOLD       0.15f
#define PROMO_CASH_VALUE 200000.0f

// Room Status
#define STATUS_AVAILABLE 0
#define STATUS_OCCUPIED  1

// Room Types
#define TYPE_SINGLE     1
#define TYPE_DOUBLE     2
#define TYPE_VIP        3

// File names
#define FILE_ROOMS      "rooms.dat"
#define FILE_INVOICES   "invoices.dat"

// ======================================================
// PART 2: STRUCTS
// ======================================================

typedef struct {
    char fullName[50];
    char idCard[15];
    char phoneNumber[12];
    int  membershipLevel;
    char email[50];
    char nationality[30];
} Customer;

typedef struct {
    char  serviceName[30];
    float unitPrice;
    int   quantity;
    float subTotal;
} Service;

typedef struct {
    char  couponCode[30];
    float cashDiscount;
    float goldDiscountRate;
    float silverDiscountRate;
} DiscountSystem;

typedef struct {
    int      roomNumber;
    int      roomType;
    float    basePrice;
    int      status;
    Customer currentGuest;
    char     checkInDate[20];
} Room;
typedef struct {
    char    invoiceID[15];
    int     targetRoomNumber;
    char    checkInDate[20];
    char    checkOutDate[20];
    int     stayDuration;
    Service usedServices[MAX_SERVICE_USED];
    int     totalServiceItems;
    float   roomTotal;
    float   serviceTotal;
    float   totalDiscount;
    float   netAmount;
} Invoice;
typedef struct RoomNode {
    Room          data;
    struct RoomNode *next;
} RoomNode;
#endif
