#pragma once

// Spatial hashing manager adapted and modified from
// https://conkerjo.wordpress.com/2009/06/13/spatial-hashing-implementation-for-fast-2d-collisions/

#include <map>
#include <vector>
#include <math.h>
#include <algorithm>

#include "Entity.h"
#include "Vec2.h"

class Spatial {
	unsigned mWidth;
	unsigned mHeight;
	unsigned mRows;
	unsigned mCols;
	unsigned mCellsize;
	std::map<unsigned, std::vector<Entity*>> mCells;

	unsigned GetCellId(Entity* e) {
		return (unsigned)((e->Center.x / mCellsize) + (e->Center.y / mCellsize)) * (mWidth / mCellsize);
	}

	void AddToCell(unsigned cellId, Entity* e) {
		// Technically, we should check if the entity already exists in the map, and if so, not add it
		// This adds a huge amount of overhead that isn't necessary, as the map is cleared and repopulated every frame anyway
		// So here, just naively add the entity
		mCells[cellId].push_back(e);
	}

	void RemoveFromCell(unsigned cellId, Entity* e) {
		auto vec = &mCells[cellId];
		vec->erase(std::remove(vec->begin(), vec->end(), e), vec->end());
	}

public:
	Spatial(int width, int height, int cellsize)
		: mWidth(width), mHeight(height), mCellsize(cellsize)
	{
		mCols = (int)ceil((float)width / cellsize);
		mRows = (int)ceil((float)height / cellsize);
		ClearCells();
	}

	~Spatial() {
		mCells.clear();
	}

	int Rows() const { return mRows; }
	int Cols() const { return mCols; }
	int Cellsize() const { return mCellsize; }
	std::map<unsigned, std::vector<Entity*>> Cells() { return mCells; }

	void ClearCells() {
		mCells.clear();
	}

	// Add an entity to the spatial map
	void Register(Entity* e) {
		auto cellId = GetCellId(e);
		AddToCell(cellId, e);
	}

	// Remove an emeity from the spatial map
	void Unregister(Entity* e) {
		auto cellId = GetCellId(e);
		RemoveFromCell(cellId, e);
	}

	// Get a list of all entitys in the same cell as the given entity
	std::vector<Entity*> GetNearby(Entity* e) {
		auto cellId = GetCellId(e);
		return mCells[cellId];
	}
};