#pragma once
#include <map>
#include <vector>
#include "Utils.hpp"

using namespace std;
using namespace utils;

struct Terrain {
	Terrain(Resource _res, int _num, bool _ladron) :	res(_res),
														num(_num),
														ladron(_ladron){
	}

	Terrain() = default;

	Resource res;
	int num;
	bool ladron;
};

class Node {
public:
	Node() {

	}

	Node(int _type) : type(_type){
		building = NONE;
		constructed = false;
		idCons = -1;
	}

	int type; //0 - Nodes / 1 - Edges
	vector<Terrain*> ter;
	unsigned short idCons;
	Structures building;
	bool constructed;
	vector<Node*> adyacent;
};

void SetTerrain(Node* _nodes[], Terrain* _terreno[]) {
	_nodes[0]->ter.push_back(_terreno[0]);
	_nodes[1]->ter.push_back(_terreno[1]);
	_nodes[2]->ter.push_back(_terreno[2]);
	_nodes[3]->ter.push_back(_terreno[0]);
	_nodes[4]->ter.push_back(_terreno[0]);
	_nodes[4]->ter.push_back(_terreno[1]);
	_nodes[5]->ter.push_back(_terreno[1]);
	_nodes[5]->ter.push_back(_terreno[2]);
	_nodes[6]->ter.push_back(_terreno[2]);
	_nodes[7]->ter.push_back(_terreno[0]);
	_nodes[7]->ter.push_back(_terreno[3]);
	_nodes[8]->ter.push_back(_terreno[0]);
	_nodes[8]->ter.push_back(_terreno[1]);
	_nodes[8]->ter.push_back(_terreno[4]);
	_nodes[9]->ter.push_back(_terreno[1]);
	_nodes[9]->ter.push_back(_terreno[2]);
	_nodes[9]->ter.push_back(_terreno[5]);
	_nodes[10]->ter.push_back(_terreno[2]);
	_nodes[10]->ter.push_back(_terreno[6]);
	_nodes[11]->ter.push_back(_terreno[4]);
	_nodes[12]->ter.push_back(_terreno[4]);
	_nodes[12]->ter.push_back(_terreno[0]);
	_nodes[12]->ter.push_back(_terreno[5]);
	_nodes[13]->ter.push_back(_terreno[1]);
	_nodes[13]->ter.push_back(_terreno[4]);
	_nodes[13]->ter.push_back(_terreno[5]);
	_nodes[14]->ter.push_back(_terreno[2]);
	_nodes[14]->ter.push_back(_terreno[5]);
	_nodes[14]->ter.push_back(_terreno[6]);
	_nodes[15]->ter.push_back(_terreno[6]);
	_nodes[16]->ter.push_back(_terreno[3]);
	_nodes[16]->ter.push_back(_terreno[7]);
	_nodes[17]->ter.push_back(_terreno[3]);
	_nodes[17]->ter.push_back(_terreno[4]);
	_nodes[17]->ter.push_back(_terreno[8]);
	_nodes[18]->ter.push_back(_terreno[4]);
	_nodes[18]->ter.push_back(_terreno[5]);
	_nodes[18]->ter.push_back(_terreno[9]);
	_nodes[19]->ter.push_back(_terreno[5]);
	_nodes[19]->ter.push_back(_terreno[6]);
	_nodes[19]->ter.push_back(_terreno[10]);
	_nodes[20]->ter.push_back(_terreno[6]);
	_nodes[20]->ter.push_back(_terreno[11]);
	_nodes[21]->ter.push_back(_terreno[7]);
	_nodes[22]->ter.push_back(_terreno[3]);
	_nodes[22]->ter.push_back(_terreno[7]);
	_nodes[22]->ter.push_back(_terreno[8]);
	_nodes[23]->ter.push_back(_terreno[4]);
	_nodes[23]->ter.push_back(_terreno[8]);
	_nodes[23]->ter.push_back(_terreno[9]);
	_nodes[24]->ter.push_back(_terreno[5]);
	_nodes[24]->ter.push_back(_terreno[9]);
	_nodes[24]->ter.push_back(_terreno[10]);
	_nodes[25]->ter.push_back(_terreno[6]);
	_nodes[25]->ter.push_back(_terreno[10]);
	_nodes[25]->ter.push_back(_terreno[11]);
	_nodes[26]->ter.push_back(_terreno[11]);
	_nodes[27]->ter.push_back(_terreno[7]);
	_nodes[28]->ter.push_back(_terreno[7]);
	_nodes[28]->ter.push_back(_terreno[8]);
	_nodes[28]->ter.push_back(_terreno[12]);
	_nodes[29]->ter.push_back(_terreno[8]);
	_nodes[29]->ter.push_back(_terreno[9]);
	_nodes[29]->ter.push_back(_terreno[13]);
	_nodes[30]->ter.push_back(_terreno[9]);
	_nodes[30]->ter.push_back(_terreno[10]);
	_nodes[30]->ter.push_back(_terreno[14]);
	_nodes[31]->ter.push_back(_terreno[10]);
	_nodes[31]->ter.push_back(_terreno[11]);
	_nodes[31]->ter.push_back(_terreno[15]);
	_nodes[32]->ter.push_back(_terreno[11]);
	_nodes[33]->ter.push_back(_terreno[7]);
	_nodes[33]->ter.push_back(_terreno[12]);
	_nodes[34]->ter.push_back(_terreno[8]);
	_nodes[34]->ter.push_back(_terreno[12]);
	_nodes[34]->ter.push_back(_terreno[13]);
	_nodes[35]->ter.push_back(_terreno[9]);
	_nodes[35]->ter.push_back(_terreno[13]);
	_nodes[35]->ter.push_back(_terreno[14]);
	_nodes[36]->ter.push_back(_terreno[10]);
	_nodes[36]->ter.push_back(_terreno[14]);
	_nodes[36]->ter.push_back(_terreno[15]);
	_nodes[37]->ter.push_back(_terreno[11]);
	_nodes[37]->ter.push_back(_terreno[15]);
	_nodes[38]->ter.push_back(_terreno[12]);
	_nodes[39]->ter.push_back(_terreno[12]);
	_nodes[39]->ter.push_back(_terreno[13]);
	_nodes[39]->ter.push_back(_terreno[16]);
	_nodes[40]->ter.push_back(_terreno[13]);
	_nodes[40]->ter.push_back(_terreno[14]);
	_nodes[40]->ter.push_back(_terreno[17]);
	_nodes[41]->ter.push_back(_terreno[14]);
	_nodes[41]->ter.push_back(_terreno[15]);
	_nodes[41]->ter.push_back(_terreno[18]);
	_nodes[42]->ter.push_back(_terreno[15]);
	_nodes[43]->ter.push_back(_terreno[12]);
	_nodes[43]->ter.push_back(_terreno[16]);
	_nodes[44]->ter.push_back(_terreno[13]);
	_nodes[44]->ter.push_back(_terreno[16]);
	_nodes[44]->ter.push_back(_terreno[17]);
	_nodes[45]->ter.push_back(_terreno[14]);
	_nodes[45]->ter.push_back(_terreno[17]);
	_nodes[45]->ter.push_back(_terreno[18]);
	_nodes[46]->ter.push_back(_terreno[15]);
	_nodes[46]->ter.push_back(_terreno[18]);
	_nodes[47]->ter.push_back(_terreno[16]);
	_nodes[48]->ter.push_back(_terreno[16]);
	_nodes[48]->ter.push_back(_terreno[17]);
	_nodes[49]->ter.push_back(_terreno[17]);
	_nodes[49]->ter.push_back(_terreno[18]);
	_nodes[50]->ter.push_back(_terreno[18]);
	_nodes[51]->ter.push_back(_terreno[16]);
	_nodes[52]->ter.push_back(_terreno[17]);
	_nodes[53]->ter.push_back(_terreno[18]);
}

void SetAdyacentEdgeToNode(Node* _edges[], Node* _nodes[]) {
	_nodes[0]->adyacent.push_back(_edges[0]);
	_nodes[0]->adyacent.push_back(_edges[1]);
	_nodes[1]->adyacent.push_back(_edges[2]);
	_nodes[1]->adyacent.push_back(_edges[3]);
	_nodes[2]->adyacent.push_back(_edges[4]);
	_nodes[2]->adyacent.push_back(_edges[5]);
	_nodes[3]->adyacent.push_back(_edges[0]);
	_nodes[3]->adyacent.push_back(_edges[6]);
	_nodes[4]->adyacent.push_back(_edges[1]);
	_nodes[4]->adyacent.push_back(_edges[2]);
	_nodes[4]->adyacent.push_back(_edges[7]);
	_nodes[5]->adyacent.push_back(_edges[3]);
	_nodes[5]->adyacent.push_back(_edges[4]);
	_nodes[5]->adyacent.push_back(_edges[8]);
	_nodes[6]->adyacent.push_back(_edges[5]);
	_nodes[6]->adyacent.push_back(_edges[9]);
	_nodes[7]->adyacent.push_back(_edges[6]);
	_nodes[7]->adyacent.push_back(_edges[10]);
	_nodes[7]->adyacent.push_back(_edges[11]);
	_nodes[8]->adyacent.push_back(_edges[7]);
	_nodes[8]->adyacent.push_back(_edges[12]);
	_nodes[8]->adyacent.push_back(_edges[13]);
	_nodes[9]->adyacent.push_back(_edges[8]);
	_nodes[9]->adyacent.push_back(_edges[14]);
	_nodes[9]->adyacent.push_back(_edges[15]);
	_nodes[10]->adyacent.push_back(_edges[9]);
	_nodes[10]->adyacent.push_back(_edges[16]);
	_nodes[10]->adyacent.push_back(_edges[17]);
	_nodes[11]->adyacent.push_back(_edges[10]);
	_nodes[11]->adyacent.push_back(_edges[18]);
	_nodes[12]->adyacent.push_back(_edges[11]);
	_nodes[12]->adyacent.push_back(_edges[12]);
	_nodes[12]->adyacent.push_back(_edges[19]);
	_nodes[13]->adyacent.push_back(_edges[13]);
	_nodes[13]->adyacent.push_back(_edges[14]);
	_nodes[13]->adyacent.push_back(_edges[20]);
	_nodes[14]->adyacent.push_back(_edges[15]);
	_nodes[14]->adyacent.push_back(_edges[16]);
	_nodes[14]->adyacent.push_back(_edges[21]);
	_nodes[15]->adyacent.push_back(_edges[17]);
	_nodes[15]->adyacent.push_back(_edges[22]);
	_nodes[16]->adyacent.push_back(_edges[18]);
	_nodes[16]->adyacent.push_back(_edges[23]);
	_nodes[16]->adyacent.push_back(_edges[24]);
	_nodes[17]->adyacent.push_back(_edges[19]);
	_nodes[17]->adyacent.push_back(_edges[25]);
	_nodes[17]->adyacent.push_back(_edges[26]);
	_nodes[18]->adyacent.push_back(_edges[20]);
	_nodes[18]->adyacent.push_back(_edges[27]);
	_nodes[18]->adyacent.push_back(_edges[28]);
	_nodes[19]->adyacent.push_back(_edges[21]);
	_nodes[19]->adyacent.push_back(_edges[29]);
	_nodes[19]->adyacent.push_back(_edges[30]);
	_nodes[20]->adyacent.push_back(_edges[22]);
	_nodes[20]->adyacent.push_back(_edges[31]);
	_nodes[20]->adyacent.push_back(_edges[32]);
	_nodes[21]->adyacent.push_back(_edges[23]);
	_nodes[21]->adyacent.push_back(_edges[33]);
	_nodes[22]->adyacent.push_back(_edges[24]);
	_nodes[22]->adyacent.push_back(_edges[25]);
	_nodes[22]->adyacent.push_back(_edges[34]);
	_nodes[23]->adyacent.push_back(_edges[26]);
	_nodes[23]->adyacent.push_back(_edges[27]);
	_nodes[23]->adyacent.push_back(_edges[35]);
	_nodes[24]->adyacent.push_back(_edges[28]);
	_nodes[24]->adyacent.push_back(_edges[29]);
	_nodes[24]->adyacent.push_back(_edges[36]);
	_nodes[25]->adyacent.push_back(_edges[30]);
	_nodes[25]->adyacent.push_back(_edges[31]);
	_nodes[25]->adyacent.push_back(_edges[37]);
	_nodes[26]->adyacent.push_back(_edges[32]);
	_nodes[26]->adyacent.push_back(_edges[38]);
	_nodes[27]->adyacent.push_back(_edges[33]);
	_nodes[27]->adyacent.push_back(_edges[39]);
	_nodes[28]->adyacent.push_back(_edges[34]);
	_nodes[28]->adyacent.push_back(_edges[40]);
	_nodes[28]->adyacent.push_back(_edges[41]);
	_nodes[29]->adyacent.push_back(_edges[35]);
	_nodes[29]->adyacent.push_back(_edges[42]);
	_nodes[29]->adyacent.push_back(_edges[43]);
	_nodes[30]->adyacent.push_back(_edges[36]);
	_nodes[30]->adyacent.push_back(_edges[44]);
	_nodes[30]->adyacent.push_back(_edges[45]);
	_nodes[31]->adyacent.push_back(_edges[37]);
	_nodes[31]->adyacent.push_back(_edges[46]);
	_nodes[31]->adyacent.push_back(_edges[47]);
	_nodes[32]->adyacent.push_back(_edges[38]);
	_nodes[32]->adyacent.push_back(_edges[48]);
	_nodes[33]->adyacent.push_back(_edges[39]);
	_nodes[33]->adyacent.push_back(_edges[40]);
	_nodes[33]->adyacent.push_back(_edges[49]);
	_nodes[34]->adyacent.push_back(_edges[41]);
	_nodes[34]->adyacent.push_back(_edges[42]);
	_nodes[34]->adyacent.push_back(_edges[50]);
	_nodes[35]->adyacent.push_back(_edges[43]);
	_nodes[35]->adyacent.push_back(_edges[44]);
	_nodes[35]->adyacent.push_back(_edges[51]);
	_nodes[36]->adyacent.push_back(_edges[45]);
	_nodes[36]->adyacent.push_back(_edges[46]);
	_nodes[36]->adyacent.push_back(_edges[52]);
	_nodes[37]->adyacent.push_back(_edges[47]);
	_nodes[37]->adyacent.push_back(_edges[48]);
	_nodes[37]->adyacent.push_back(_edges[53]);
	_nodes[38]->adyacent.push_back(_edges[49]);
	_nodes[38]->adyacent.push_back(_edges[54]);
	_nodes[39]->adyacent.push_back(_edges[50]);
	_nodes[39]->adyacent.push_back(_edges[55]);
	_nodes[39]->adyacent.push_back(_edges[56]);
	_nodes[40]->adyacent.push_back(_edges[51]);
	_nodes[40]->adyacent.push_back(_edges[57]);
	_nodes[40]->adyacent.push_back(_edges[58]);
	_nodes[41]->adyacent.push_back(_edges[52]);
	_nodes[41]->adyacent.push_back(_edges[59]);
	_nodes[41]->adyacent.push_back(_edges[60]);
	_nodes[42]->adyacent.push_back(_edges[53]);
	_nodes[42]->adyacent.push_back(_edges[61]);
	_nodes[43]->adyacent.push_back(_edges[54]);
	_nodes[43]->adyacent.push_back(_edges[55]);
	_nodes[43]->adyacent.push_back(_edges[62]);
	_nodes[44]->adyacent.push_back(_edges[56]);
	_nodes[44]->adyacent.push_back(_edges[57]);
	_nodes[44]->adyacent.push_back(_edges[63]);
	_nodes[45]->adyacent.push_back(_edges[58]);
	_nodes[45]->adyacent.push_back(_edges[59]);
	_nodes[45]->adyacent.push_back(_edges[64]);
	_nodes[46]->adyacent.push_back(_edges[60]);
	_nodes[46]->adyacent.push_back(_edges[61]);
	_nodes[46]->adyacent.push_back(_edges[65]);
	_nodes[47]->adyacent.push_back(_edges[62]);
	_nodes[47]->adyacent.push_back(_edges[66]);
	_nodes[48]->adyacent.push_back(_edges[63]);
	_nodes[48]->adyacent.push_back(_edges[67]);
	_nodes[48]->adyacent.push_back(_edges[68]);
	_nodes[49]->adyacent.push_back(_edges[64]);
	_nodes[49]->adyacent.push_back(_edges[69]);
	_nodes[49]->adyacent.push_back(_edges[70]);
	_nodes[50]->adyacent.push_back(_edges[65]);
	_nodes[50]->adyacent.push_back(_edges[71]); 
	_nodes[51]->adyacent.push_back(_edges[66]);
	_nodes[51]->adyacent.push_back(_edges[67]);
	_nodes[52]->adyacent.push_back(_edges[68]);
	_nodes[52]->adyacent.push_back(_edges[69]);
	_nodes[53]->adyacent.push_back(_edges[70]);
	_nodes[53]->adyacent.push_back(_edges[71]);
}

void SetAdyacentNodeToEdge(Node* _nodes[],Node* _edges[]) {
	_edges[0]->adyacent.push_back(_nodes[0]);
	_edges[0]->adyacent.push_back(_nodes[3]);
	_edges[1]->adyacent.push_back(_nodes[0]);
	_edges[1]->adyacent.push_back(_nodes[4]);
	_edges[2]->adyacent.push_back(_nodes[1]);
	_edges[2]->adyacent.push_back(_nodes[4]);
	_edges[3]->adyacent.push_back(_nodes[1]);
	_edges[3]->adyacent.push_back(_nodes[5]);
	_edges[4]->adyacent.push_back(_nodes[2]);
	_edges[4]->adyacent.push_back(_nodes[5]);
	_edges[5]->adyacent.push_back(_nodes[2]);
	_edges[5]->adyacent.push_back(_nodes[6]);
	_edges[6]->adyacent.push_back(_nodes[3]);
	_edges[6]->adyacent.push_back(_nodes[7]);
	_edges[7]->adyacent.push_back(_nodes[4]);
	_edges[7]->adyacent.push_back(_nodes[8]);
	_edges[8]->adyacent.push_back(_nodes[5]);
	_edges[8]->adyacent.push_back(_nodes[9]);
	_edges[9]->adyacent.push_back(_nodes[6]);
	_edges[9]->adyacent.push_back(_nodes[10]);
	_edges[10]->adyacent.push_back(_nodes[7]);
	_edges[10]->adyacent.push_back(_nodes[11]);
	_edges[11]->adyacent.push_back(_nodes[7]);
	_edges[11]->adyacent.push_back(_nodes[12]);
	_edges[12]->adyacent.push_back(_nodes[8]);
	_edges[12]->adyacent.push_back(_nodes[12]);
	_edges[13]->adyacent.push_back(_nodes[8]);
	_edges[13]->adyacent.push_back(_nodes[13]);
	_edges[14]->adyacent.push_back(_nodes[9]);
	_edges[14]->adyacent.push_back(_nodes[13]);
	_edges[15]->adyacent.push_back(_nodes[9]);
	_edges[15]->adyacent.push_back(_nodes[14]);
	_edges[16]->adyacent.push_back(_nodes[10]);
	_edges[16]->adyacent.push_back(_nodes[14]);
	_edges[17]->adyacent.push_back(_nodes[10]);
	_edges[17]->adyacent.push_back(_nodes[15]);
	_edges[18]->adyacent.push_back(_nodes[11]);
	_edges[18]->adyacent.push_back(_nodes[16]);
	_edges[19]->adyacent.push_back(_nodes[12]);
	_edges[19]->adyacent.push_back(_nodes[17]);
	_edges[20]->adyacent.push_back(_nodes[13]);
	_edges[20]->adyacent.push_back(_nodes[18]);
	_edges[21]->adyacent.push_back(_nodes[14]);
	_edges[21]->adyacent.push_back(_nodes[19]);
	_edges[22]->adyacent.push_back(_nodes[15]);
	_edges[22]->adyacent.push_back(_nodes[20]);
	_edges[23]->adyacent.push_back(_nodes[16]);
	_edges[23]->adyacent.push_back(_nodes[21]);
	_edges[24]->adyacent.push_back(_nodes[16]);
	_edges[24]->adyacent.push_back(_nodes[22]);
	_edges[25]->adyacent.push_back(_nodes[17]);
	_edges[25]->adyacent.push_back(_nodes[22]);
	_edges[26]->adyacent.push_back(_nodes[17]);
	_edges[26]->adyacent.push_back(_nodes[23]);
	_edges[27]->adyacent.push_back(_nodes[18]);
	_edges[27]->adyacent.push_back(_nodes[23]);
	_edges[28]->adyacent.push_back(_nodes[18]);
	_edges[28]->adyacent.push_back(_nodes[24]);
	_edges[29]->adyacent.push_back(_nodes[19]);
	_edges[29]->adyacent.push_back(_nodes[24]);
	_edges[30]->adyacent.push_back(_nodes[19]);
	_edges[30]->adyacent.push_back(_nodes[25]);
	_edges[31]->adyacent.push_back(_nodes[20]);
	_edges[31]->adyacent.push_back(_nodes[25]);
	_edges[32]->adyacent.push_back(_nodes[20]);
	_edges[32]->adyacent.push_back(_nodes[26]);
	_edges[33]->adyacent.push_back(_nodes[21]);
	_edges[33]->adyacent.push_back(_nodes[27]);
	_edges[34]->adyacent.push_back(_nodes[22]);
	_edges[34]->adyacent.push_back(_nodes[28]);
	_edges[35]->adyacent.push_back(_nodes[23]);
	_edges[35]->adyacent.push_back(_nodes[29]);
	_edges[36]->adyacent.push_back(_nodes[24]);
	_edges[36]->adyacent.push_back(_nodes[30]);
	_edges[37]->adyacent.push_back(_nodes[25]);
	_edges[37]->adyacent.push_back(_nodes[31]);
	_edges[38]->adyacent.push_back(_nodes[26]);
	_edges[38]->adyacent.push_back(_nodes[32]);
	_edges[39]->adyacent.push_back(_nodes[27]);
	_edges[39]->adyacent.push_back(_nodes[33]);
	_edges[40]->adyacent.push_back(_nodes[28]);
	_edges[40]->adyacent.push_back(_nodes[33]);
	_edges[41]->adyacent.push_back(_nodes[28]);
	_edges[41]->adyacent.push_back(_nodes[34]);
	_edges[42]->adyacent.push_back(_nodes[29]);
	_edges[42]->adyacent.push_back(_nodes[34]);
	_edges[43]->adyacent.push_back(_nodes[29]);
	_edges[43]->adyacent.push_back(_nodes[35]);
	_edges[44]->adyacent.push_back(_nodes[30]);
	_edges[44]->adyacent.push_back(_nodes[35]);
	_edges[45]->adyacent.push_back(_nodes[30]);
	_edges[45]->adyacent.push_back(_nodes[36]);
	_edges[46]->adyacent.push_back(_nodes[31]);
	_edges[46]->adyacent.push_back(_nodes[36]);
	_edges[47]->adyacent.push_back(_nodes[31]);
	_edges[47]->adyacent.push_back(_nodes[37]);
	_edges[48]->adyacent.push_back(_nodes[32]);
	_edges[48]->adyacent.push_back(_nodes[37]);
	_edges[49]->adyacent.push_back(_nodes[33]);
	_edges[49]->adyacent.push_back(_nodes[38]);
	_edges[50]->adyacent.push_back(_nodes[34]);
	_edges[50]->adyacent.push_back(_nodes[39]);
	_edges[51]->adyacent.push_back(_nodes[35]);
	_edges[51]->adyacent.push_back(_nodes[40]);
	_edges[52]->adyacent.push_back(_nodes[36]);
	_edges[52]->adyacent.push_back(_nodes[41]);
	_edges[53]->adyacent.push_back(_nodes[37]);
	_edges[53]->adyacent.push_back(_nodes[42]);
	_edges[54]->adyacent.push_back(_nodes[38]);
	_edges[54]->adyacent.push_back(_nodes[43]);
	_edges[55]->adyacent.push_back(_nodes[39]);
	_edges[55]->adyacent.push_back(_nodes[43]);
	_edges[56]->adyacent.push_back(_nodes[39]);
	_edges[56]->adyacent.push_back(_nodes[44]);
	_edges[57]->adyacent.push_back(_nodes[40]);
	_edges[57]->adyacent.push_back(_nodes[44]);
	_edges[58]->adyacent.push_back(_nodes[40]);
	_edges[58]->adyacent.push_back(_nodes[45]);
	_edges[59]->adyacent.push_back(_nodes[41]);
	_edges[59]->adyacent.push_back(_nodes[45]);
	_edges[60]->adyacent.push_back(_nodes[41]);
	_edges[60]->adyacent.push_back(_nodes[46]);
	_edges[61]->adyacent.push_back(_nodes[42]);
	_edges[61]->adyacent.push_back(_nodes[46]);
	_edges[62]->adyacent.push_back(_nodes[43]);
	_edges[62]->adyacent.push_back(_nodes[47]);
	_edges[63]->adyacent.push_back(_nodes[44]);
	_edges[63]->adyacent.push_back(_nodes[48]);
	_edges[64]->adyacent.push_back(_nodes[45]);
	_edges[64]->adyacent.push_back(_nodes[49]);
	_edges[65]->adyacent.push_back(_nodes[46]);
	_edges[65]->adyacent.push_back(_nodes[50]);
	_edges[66]->adyacent.push_back(_nodes[47]);
	_edges[66]->adyacent.push_back(_nodes[51]);
	_edges[67]->adyacent.push_back(_nodes[48]);
	_edges[67]->adyacent.push_back(_nodes[51]);
	_edges[68]->adyacent.push_back(_nodes[48]);
	_edges[68]->adyacent.push_back(_nodes[52]);
	_edges[69]->adyacent.push_back(_nodes[49]);
	_edges[69]->adyacent.push_back(_nodes[52]);
	_edges[70]->adyacent.push_back(_nodes[49]);
	_edges[70]->adyacent.push_back(_nodes[53]);
	_edges[70]->adyacent.push_back(_nodes[50]);
	_edges[70]->adyacent.push_back(_nodes[53]);
}

class Map {
public:
	Map() {
		//Create the terrain
		//De momento no lo creamos de forma random, mas adelante o si sobra tiempo
		terreno[0] = new Terrain(WOOD, 2, false);
		terreno[1] = new Terrain(STEEL, 3, false);
		terreno[2] = new Terrain(CLAY, 4, false);
		terreno[3] = new Terrain(SHEEP, 5, false);
		terreno[4] = new Terrain(STRAW, 6, false);
		terreno[5] = new Terrain(WOOD, 8, false);
		terreno[6] = new Terrain(CLAY, 9, false);
		terreno[7] = new Terrain(STRAW, 10, false);
		terreno[8] = new Terrain(SHEEP, 11, false);
		terreno[9] = new Terrain(DESERT, 0, true);
		terreno[10] = new Terrain(STEEL, 12, false);
		terreno[11] = new Terrain(SHEEP, 11, false);
		terreno[12] = new Terrain(WOOD, 10, false);
		terreno[13] = new Terrain(STRAW, 9, false);
		terreno[14] = new Terrain(STEEL, 8, false);
		terreno[15] = new Terrain(CLAY, 6, false);
		terreno[16] = new Terrain(WOOD, 5, false);
		terreno[17] = new Terrain(SHEEP, 4, false);
		terreno[18] = new Terrain(STRAW, 3, false);
		//Create the empty nodes
		for (int i = 0; i < NUM_NODES; i++) {
			nodes[i] = new Node(0);
		}
		//Create the empty edges
		for (int i = 0; i < NUM_EDGES; i++) {
			edges[i] = new Node(1);
		}
		SetTerrain(nodes, terreno);
		SetAdyacentEdgeToNode(edges, nodes);
		SetAdyacentNodeToEdge(nodes, edges);
	}

	bool CanConstructNodeFirst(int node);
	bool CanConstructNode(int id, int node);
	bool CanConstructBridge(int id, int edge);
	bool CanUpgrade(int id, int node);

	void Construct(int id, Structures build, int node);
	void Upgrade(int node);
	int CalculatePoints(int id);

	//Almacena el recurso del exacell y el numero que le corresponde
	Terrain* terreno[NUM_CELLS];
	//Almacena los nodos del tablero donde se puede construir con sus respectivos terrenos y edges adyacentes
	Node* nodes[NUM_NODES];
	//Almacena los edges con sus nodos adyacentes
	Node* edges[NUM_EDGES];
};

bool Map::CanConstructNodeFirst(int node) {
	if (nodes[node]->constructed) {
		return false;
	}
	else {
		for each(Node* n in nodes[node]->adyacent) { // Revisa los caminos de alrededor
			for each(Node* e in n->adyacent) { // Revisa que no haya nada construido a 2 casillas (REGLA)
				if (e->constructed)
					return false;
			}
		}
		return true;
	}
}

bool Map::CanConstructNode(int id, int node) {
	if (nodes[node]->constructed) {
		return false;
	}
	else {
		bool bridge = false;
		for each(Node* n in nodes[node]->adyacent) { // Revisa los caminos de alrededor
			if (n->constructed && n->idCons == id) { // Mira si hay algun camino tuyo en los adyacentes al nodo
				bridge = true;
			}
			for each(Node* e in n->adyacent) { // Revisa que no haya nada construido a 2 casillas (REGLA)
				if (e->constructed)
					return false;
			}
		}
		if (bridge)
			return true;
		else
			return false;
	}
}

bool Map::CanConstructBridge(int id, int edge) {
	if (edges[edge]->constructed) {
		return false;
	}
	else {
		for each(Node* b in edges[edge]->adyacent) {
			if (b->constructed && b->idCons == id) {
				return true;
			}
		}
		return false;
	}
}

bool Map::CanUpgrade(int id, int node) {
	if (nodes[node]->constructed && nodes[node]->idCons == id && nodes[node]->building == utils::TOWN) {
		return true;
	}
	else {
		return false;
	}
}

void Map::Construct(int id, Structures build, int node) {
	nodes[node]->idCons = id;
	nodes[node]->building = build;
	nodes[node]->constructed = true;
}

void Map::Upgrade(int node) {
	nodes[node]->building = utils::CITY;
}

int Map::CalculatePoints(int id) {
	int points = 0;
	for (int i = 0; i < NUM_NODES; i++) {
		if (nodes[i]->idCons == id) {
			if (nodes[i]->building == utils::TOWN)
				points += 1;
			else if (nodes[i]->building == utils::CITY)
				points += 2;
		}
	}
	return points;
}