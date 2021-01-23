#pragma once
#include<vector>
//#include "GameObjects.h"
using namespace std;

class GameObject;

class CGridObjects
{
private:
	const int GRID_CELL_WIDTH = 64;
	const int GRID_CELL_HEIGHT = 64;

	int rowsCount, colsCount;

	/// <summary>
	/// Large objects are the ones that cannot fit into any cells
	/// </summary>
	vector<GameObject*> largeObjs;

	/// [row][col][obj]
	vector<vector<vector<GameObject*>>> cells;

	void CalcGridRange(int areaX, int areaY, int areaWidth, int areaHeight, int& leftColumn, int& topRow, int& rightColumn, int& bottomRow);

	bool IsLargeObject(GameObject* obj);

	/// <summary>
	/// CuteTN Note:
	/// remove game object from a specific cell node. This is also used to remove a game object from its old cell after updating to a new cell
	/// </summary>
	/// <param name="obj"></param>
	/// <param name="row"></param>
	/// <param name="col"></param>
	/// <param name="deleteAfterRemoving"></param>
	void RemoveFromCell(GameObject* obj, int row, int col, bool deleteAfterRemoving);

	void RemoveFromLargeObjects(GameObject* obj, bool deleteAfterRemoving);


public:
	CGridObjects() {}
	CGridObjects(int sectionWidth, int sectionHeight);

	void CalcCell(int& row, int& col, int x, int y);
	void CalcCell(int& row, int& col, GameObject* obj);

	void Add(GameObject* obj);
	void Remove(GameObject* obj, bool deleteAfterRemoving);

	/// <summary>
	/// this function tries to seek the object any where on the grid. It cannot hide :^)
	/// </summary>
	/// <param name="obj"></param>
	void SeekAndRemove(GameObject* obj, bool deleteAfterRemoving);
	void Clear();

	vector<GameObject*> GetObjectsInArea(int areaX, int areaY, int areaWidth, int areaHeight);
	vector<GameObject*> GetAllObjects();
	void ClearObjectsInArea(int dt, int areaX, int areaY, int areaWidth, int areaHeight);
};

typedef CGridObjects* LPGRIDOBJECTS;
