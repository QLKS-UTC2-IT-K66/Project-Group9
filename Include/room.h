#ifndef ROOM_H
#define ROOM_H

#include "model.h"

RoomNode*   createRoomNode(Room r);
void        addRoom(RoomNode **head);
void        editRoom(RoomNode *head);
void        deleteRoom(RoomNode **head);
void        showAvailable(RoomNode *head);
void        showAllRooms(RoomNode *head);
void        searchRoom(RoomNode *head);
RoomNode*   findRoom(RoomNode *head, int roomNumber);
void        freeRoomList(RoomNode **head);
const char* getRoomTypeName(int roomType);
int         countRooms(RoomNode *head);
int         countAvailableRooms(RoomNode *head);
void        printRoomStats(RoomNode *head);

#endif
