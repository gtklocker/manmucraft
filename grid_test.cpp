#include <iostream>
#include <cstdlib>
#include <cassert>

#include "grid.h"

using namespace std;

bool testTransformRealToGreedy() {
	const int GRID_SIZE = 10;
	const int TILE_SIZE = 1;

	Grid grid(GRID_SIZE, TILE_SIZE);

	const int TESTS = 3;
	RealCoords input[TESTS] = {
		(RealCoords){
			-.25,
			0,
			-.25
		},
		(RealCoords){
			0,
			0,
			1.75
		},
		(RealCoords){
			0,
			0,
			10
		},
	};

	GreedyCoords ans[TESTS] = {
		(GreedyCoords){
			0,
			0,
			0
		},
		(GreedyCoords){
			0,
			0,
			2
		},
		(GreedyCoords){
			0,
			0,
			10
		}
	};

	for (int i = 0; i < TESTS; ++i) {
		GreedyCoords output = grid.transformRealToGreedy(input[i]);
		cout << "asserting that " << output.x << " = " << ans[i].x << endl;
		assert(output.x == ans[i].x);
		cout << "asserting that " << output.y << " = " << ans[i].y << endl;
		assert(output.y == ans[i].y);
		cout << "asserting that " << output.z << " = " << ans[i].z << endl;
		assert(output.z == ans[i].z);
	}

	return true;
}

bool testTransformGreedyToReal() {
	const int GRID_SIZE = 10;
	const float TILE_SIZE = .5;

	Grid grid(GRID_SIZE, TILE_SIZE);

	const int TESTS = 3;
	GreedyCoords input[TESTS] = {
		(GreedyCoords){
			0,
			0,
			0
		},
		(GreedyCoords){
			1,
			0,
			0
		},
		(GreedyCoords){
			0,
			0,
			2
		}
	};

	RealCoords ans[TESTS] = {
		(RealCoords){
			0,
			0,
			0
		},
		(RealCoords){
			.5,
			0,
			0

		},
		(RealCoords){
			0,
			0,
			1
		},
	};


	for (int i = 0; i < TESTS; ++i) {
		RealCoords output = grid.transformGreedyToReal(input[i]);
		cout << "asserting that " << output.x << " = " << ans[i].x << endl;
		assert(output.x == ans[i].x);
		cout << "asserting that " << output.y << " = " << ans[i].y << endl;
		assert(output.y == ans[i].y);
		cout << "asserting that " << output.z << " = " << ans[i].z << endl;
		assert(output.z == ans[i].z);
	}

	return true;
}

int main() {
	assert(testTransformRealToGreedy());
	assert(testTransformGreedyToReal());
}
