// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
// C++ program for solving exact cover problem
// using DLX (Dancing Links) technique
#include <iostream>
#include <vector>
#include <memory>
#define MAX_ROW 100
#define MAX_COL 100

using namespace std;

class Node
{
public:
	Node() = default;
	shared_ptr<Node> left=nullptr;
	shared_ptr<Node> right=nullptr;
	shared_ptr<Node> up = nullptr;
	shared_ptr<Node> down = nullptr;
	shared_ptr<Node> column = nullptr;

	int rowID;
	int colID;
	int nodeCount;
	virtual ~Node() {};
};

class ExactCover {
public:
	ExactCover() :header(make_shared<Node>()) {}
	virtual ~ExactCover() = default;

	// Header node points to first column
	shared_ptr<Node> header;

	// Matrix to contain nodes
	Node Matrix[MAX_ROW][MAX_COL];

	// We express out problem as a boolean matrix
	bool InputMatrix[MAX_ROW][MAX_COL];

	// vector containing solutions
	vector <shared_ptr<Node>> solutions;

	// Number of rows and columns in problem matrix
	int nRow = 0, nCol = 0;


	// Functions to get next index in any direction
	// for given index (circular in nature)
	int getRight(int i) { return (i + 1) % nCol; }
	int getLeft(int i) { return (i - 1 < 0) ? nCol - 1 : i - 1; }
	int getUp(int i) { return (i - 1 < 0) ? nRow : i - 1; }
	int getDown(int i) { return (i + 1) % (nRow + 1); }

	// use inputmatrix to construct out node matrix
	// This is a Toroid
	shared_ptr<Node> createMatrix()
	{
		// One extra row for list header nodes
		for (int i = 0; i <= nRow; i++)
		{
			// for each column
			for (int j = 0; j < nCol; j++)
			{
				// If it's 1 in the problem matrix then
				// only create a node
				if (InputMatrix[i][j])
				{
					int a, b;
					// If it's 1, other than 1 in 0th row
					// then count it as node of column
					// and increment node count in column header
					if (i) Matrix[0][j].nodeCount += 1;

					// Add pointer to column header for this
					// column node
					Matrix[i][j].column = shared_ptr<Node>(&Matrix[0][j]);

					// set row and column id of this node
					Matrix[i][j].rowID = i;
					Matrix[i][j].colID = j;

					// Link the node with neighbors:
					// 1.Left pointer
					a = i; b = j;
					do { b = getLeft(b); } while (!InputMatrix[a][b] && b != j);
					Matrix[i][j].left = shared_ptr<Node>(&Matrix[i][b]);

					// 2.Right pointer
					a = i; b = j;
					do { b = getRight(b); } while (!InputMatrix[a][b] && b != j);
					Matrix[i][j].right = shared_ptr<Node>(&Matrix[i][b]);

					// 3.Up pointer
					a = i; b = j;
					do { a = getUp(a); } while (!InputMatrix[a][b] && a != i);
					Matrix[i][j].up = shared_ptr<Node>(&Matrix[a][j]);

					// 4/Down pointer
					a = i; b = j;
					do { a = getDown(a); } while (!InputMatrix[a][b] && a != i);
					Matrix[i][j].down = shared_ptr<Node>(&Matrix[a][j]);
				}
			}
		}

		// link header right pointer to column
		// header of first column
		

		header->right = shared_ptr<Node>(&Matrix[0][0]);

		// link header left pointer to column
		// header of last column
		header->left = shared_ptr<Node>(&Matrix[0][nCol - 1]);

		(Matrix[0][0]).left = header;
		(Matrix[0][nCol - 1]).right = header;
		return header;
	}

	//remove the given node
	void remove(shared_ptr<Node> node)
	{
		auto colNode = node->column;

		// unlink column header
		colNode->left->right = colNode->right;
		colNode->right->left = colNode->left;

		// Move down the column and remove each row
		// by traversing right
		for (auto row = colNode->down; row != colNode; row = row->down)
		{
			for (auto rightNode = row->right; rightNode != row;
			rightNode = rightNode->right)
			{
				rightNode->up->down = rightNode->down;
				rightNode->down->up = rightNode->up;

				// after unlinking row node, decrement the
				// node count in column header
				Matrix[0][rightNode->colID].nodeCount -= 1;
			}
		}
	}

	//resume the given node
	void resume(shared_ptr<Node> node)
	{
		auto colNode = node->column;

		// Move down the column and link back
		// each row by traversing left
		for (auto rowNode = colNode->up; rowNode != colNode; rowNode = rowNode->up)
		{
			for (auto leftNode = rowNode->left; leftNode != rowNode;
			leftNode = leftNode->left)
			{
				leftNode->up->down = leftNode;
				leftNode->down->up = leftNode;

				// after linking row node, increment the
				// node count in column header
				Matrix[0][leftNode->colID].nodeCount += 1;
			}
		}

		// link the column header from it's neighbors
		colNode->left->right = colNode;
		colNode->right->left = colNode;
	}

	//heuristic choose the column which has the minimum colum
	shared_ptr<Node> getMinColumn()
	{
		auto h = header;
		auto result = h->right;
		h = h->right->right;
		do
		{
			if (h->nodeCount < result->nodeCount)
			{
				result = h;
			}
			auto tmp = h->right;
			h = tmp;
		} while (h != header);

		return result;
	}


	void printSolutions()
	{

		for (auto i = solutions.cbegin(); i != solutions.cend(); i++)
			cout << (*i)->rowID << " ";
		cout << "\n";
	}

	// Search for exact covers
	void dancing(int k)
	{
		//we found the solution
		if (header->right == header)
		{
			printSolutions();
			return;
		}

		// choose column heuristically
		auto column = getMinColumn();
		remove(column);
		cout << "choose column:" << column->colID << endl;
		cout << "remove column:" << column->colID << endl;
		for (auto rowNode = column->down; rowNode != column;
		rowNode = rowNode->down) {
			//choose row from top to bottom
			solutions.push_back(rowNode);
			cout << "choose row:" << rowNode->rowID << endl;


			for (auto rightNode = rowNode->right; rightNode != rowNode;
			rightNode = rightNode->right) {
				remove(rightNode);
				cout << "remove col:" << rightNode->colID << endl;
			}

			// move to level k+1 (recursively)
			dancing(k + 1);

			// if solution in not possible, backtrack (uncover)
			// and remove the selected row (set) from solution
			solutions.pop_back();

			column = rowNode->column;
			for (auto leftNode = rowNode->left; leftNode != rowNode;
			leftNode = leftNode->left) {
				resume(leftNode);
				cout << "resume col:" << leftNode->colID << endl;
			}

		}
		cout << "resume column:" << column->colID << endl;
		resume(column);
	}
};

// Driver code
int main()
{
	/*
	Example problem
	zz
	X = {1,2,3,4,5,6,7}
	set-1 = {1,4,7}
	set-2 = {1,4}
	set-3 = {4,5,7}
	set-4 = {3,5,6}
	set-5 = {2,3,6,7}
	set-6 = {2,7}
	set-7 = {1,4}

	Solutions : {6 ,4, 2} and {6, 4, 7}
	*/
	ExactCover exact_cover;
	exact_cover.nRow = 7;
	exact_cover.nCol = 7;

	// initialize the problem matrix
	// ( matrix of 0 and 1) with 0
	for (int i = 0; i <= exact_cover.nRow; i++)
	{
		for (int j = 0; j<exact_cover.nCol; j++)
		{
			// if it's row 0, it consist of column
			// headers. Initialize it with 1
			if (i == 0) exact_cover.InputMatrix[i][j] = true;
			else exact_cover.InputMatrix[i][j] = false;
		}
	}

	// Manually filling up 1's
	exact_cover.InputMatrix[1][0] = true; exact_cover.InputMatrix[1][3] = true;
	exact_cover.InputMatrix[1][6] = true; exact_cover.InputMatrix[2][0] = true;
	exact_cover.InputMatrix[2][3] = true; exact_cover.InputMatrix[3][3] = true;
	exact_cover.InputMatrix[3][4] = true; exact_cover.InputMatrix[3][6] = true;
	exact_cover.InputMatrix[4][2] = true; exact_cover.InputMatrix[4][4] = true;
	exact_cover.InputMatrix[4][5] = true; exact_cover.InputMatrix[5][1] = true;
	exact_cover.InputMatrix[5][2] = true; exact_cover.InputMatrix[5][5] = true;
	exact_cover.InputMatrix[5][6] = true; exact_cover.InputMatrix[6][1] = true;
	exact_cover.InputMatrix[6][6] = true; exact_cover.InputMatrix[7][0] = true;
	exact_cover.InputMatrix[7][3] = true;

	// create 4-way linked matrix
	exact_cover.createMatrix();

	exact_cover.dancing(0);
	return 0;
}
