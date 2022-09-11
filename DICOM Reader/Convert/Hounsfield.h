#pragma once
const enum Matter {
	Air, Fat, SoftTissue, CancellousBone, CorticalBone, SubduralHematoma1, SubduralHematoma2, SubduralHematoma3,
	UnclottedBlood, ClottedBlood, Water, Urine, Bile, Abscess, Mucus, Lung, Kidney, Liver, LymphNodes, Muscle, Thymus,
	WhiteMatter, GrayMatter, CholesterolStone, BilirubinStone, NumberOfMatter
};

struct HounsfieldEntry{
	Matter id;
	struct {
		int lower, upper;
	};
};

const HounsfieldEntry HounsfieldScale[NumberOfMatter] = {
	{Air, {0, 0}},
	{Fat, {880, 910}},
	{SoftTissue, {1100, 1300}},
	{CancellousBone, {1300, 1400}},
	{CorticalBone, {1500, 2900}},
	{SubduralHematoma1, {1075, 1100}},
	{SubduralHematoma2, {1065, 1085}},
	{SubduralHematoma3, {1035, 1040}},
	{UnclottedBlood, {1013, 1050}},
	{ClottedBlood, {1050, 1075}},
	{Water, {1000, 1000}},
	{Urine, {995, 1015}},
	{Bile, {995, 1015}},
	{Abscess, {1020, 1040}},
	{Mucus, {1000, 1130}},
	{Lung, {300, 400}},
	{Kidney, {1020, 1045}},
	{Liver, {1055, 1065}},
	{LymphNodes, {1010, 1020}},
	{Muscle, {1035, 1055}},
	{Thymus, {1020, 1120}},
	{WhiteMatter, {1020, 1030}},
	{GrayMatter, {1037, 1045}},
	{CholesterolStone, {1030, 1100}},
	{BilirubinStone, {1090, 1120}}
};
