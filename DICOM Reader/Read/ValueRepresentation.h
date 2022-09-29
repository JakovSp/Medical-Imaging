/*
ValueRepresentation.h

	Each VR identifier is assigned a unique 2 byte constant 
	corresponding to their name in ASCII
*/
#pragma once

#include "pch.h"

constexpr uint16_t AE = (uint16_t)0x4541; // Application Entity
constexpr uint16_t AS = (uint16_t)0x5341; // Age String
constexpr uint16_t AT = (uint16_t)0x5441; // DEInfoVerbose Tag
constexpr uint16_t CS = (uint16_t)0x5343; // Code String
constexpr uint16_t DA = (uint16_t)0x4144; // Date
constexpr uint16_t DS = (uint16_t)0x5344; // Decimal String
constexpr uint16_t DT = (uint16_t)0x5444; // Date Time
constexpr uint16_t FD = (uint16_t)0x4446; // Float Point Double
constexpr uint16_t FL = (uint16_t)0x4c46; // Float Point Single
constexpr uint16_t IS = (uint16_t)0x5349; // Integer String
constexpr uint16_t LO = (uint16_t)0x4f4c; // Long String
constexpr uint16_t LT = (uint16_t)0x544c; // Long Text
constexpr uint16_t OB = (uint16_t)0x424f; // Other Byte
//#define OD		// Other Double
//#define OF		// Other Float
//#define OL		// Other Long
//#define OV		// Other Very Long
constexpr uint16_t OW = (uint16_t)0x574f; // Other Word
constexpr uint16_t PN = (uint16_t)0x4e50; // Person Name
constexpr uint16_t SH = (uint16_t)0x4853; // Short String
constexpr uint16_t SL = (uint16_t)0x4c53; // Signed Long
constexpr uint16_t SQ = (uint16_t)0x5153; // Sequence of Items
constexpr uint16_t SS = (uint16_t)0x5353; // Signed Short
constexpr uint16_t ST = (uint16_t)0x5453; // Short Text
//#define SV		// Signed 64-bit Very Long
constexpr uint16_t TM = (uint16_t)0x4d54; // Time
constexpr uint16_t UI = (uint16_t)0x4955; // Unique Identifier
constexpr uint16_t UL = (uint16_t)0x4c55; // Unsigned Long
//#define UN		// Unknown octet stream
//#define UR		// Universal Resource Identifier (URI)
constexpr uint16_t US = (uint16_t)0x5355; // Unsigned Short
constexpr uint16_t UT = (uint16_t)0x5455; // Unlimited Text
//#define UV		// Unsigned 64-bit Very Long

constexpr uint32_t UNDEFINED = 0xFFFFFFFF;

// std::map<uint16_t, std::type_index> StorageType = {
// 	{AE, typeid(StringDE)},// Application Entity
// 	{AS, typeid(StringDE)},// Age String
// 	{AT, typeid(StringDE)},// DEInfoVerbose Tag
// 	{CS, typeid(StringDE)},// Code String
// 	{DA, typeid(StringDE)},// Date
// 	{DS, typeid(NumericStringDE)},// Decimal String
// 	{DT, typeid(StringDE)},// Date Time
// 	{FD, typeid(BinaryDE)},// Float Point Double
// 	{FL, typeid(BinaryDE)},// Float Point Single
// 	{IS, typeid(NumericStringDE)},// Integer String
// 	{LO, typeid(StringDE)},// Long String
// 	{LT, typeid(StringDE)},// Long Text
// 	{OB, typeid(StreamDE)},// Other Byte
// 	{OW, typeid(StreamDE)},// Other Word
// 	{PN, typeid(StringDE)},// Person Name
// 	{SH, typeid(StringDE)},// Short String
// 	{SL, typeid(BinaryDE)},// Signed Long
// 	{SQ, typeid(DataSet)},// Sequence of Items
// 	{SS, typeid(BinaryDE)},// Signed Short
// 	{ST, typeid(StringDE)},// Short Text
// 	{TM, typeid(StringDE)},// Time
// 	{UI, typeid(StringDE)},// Unique Identifier
// 	{UL, typeid(BinaryDE)},// Unsigned Long
// 	{US, typeid(BinaryDE)},// Unsigned Short
// 	{UT, typeid(StringDE)}// Unlimited Text
// };
