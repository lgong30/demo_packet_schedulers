
//! class for data of the node in AVL tree
class DataField{
public:
	double mVTimeMax;
	double mDeltaWeight;
	double mDeltaRTime;
	DataField()
	{
		mVTimeMax = 0.0;
		mDeltaRTime = 0.0;
		mDeltaWeight 0.0;
	}
	DataField(double VTime,double deltaWeight)
	{
		mVTimeMax = VTime;
		mDeltaWeight = deltaWeight;
		mDeltaRTime = 0.0;
	}

};

class L_GPSSim{
	//! 
	double mOldVTime;
	//!
	double mOldRTime;
	//!
	double mSumWeight

	//!
	AVL_Tree<Node> *mpBalancedTree;
public:
	L_GPSSim()
	{
		mOldRTime = 0;
		mOldRTime = 0;
		mSumWeight = 0;

		mpBalancedTree = new AVL_Tree<Node>();
	}
	//! Function to compute the corresponding virtual time for a new real time
	double RTime2VTime(double NewRTime)
	{
		//! virtual time, real time and total weight after last event
		double oldVTime = mOldVTime,
			   oldRTime = mOldRTime,
			   oldSumWeight = mSumWeight;

	    //! obtain the root of the avl tree
		Node *pCurNode = mpBalancedTree.GetRoot();

		//! perform search on the tree
		while (!mpBalancedTree->IsLeaf(pCurNode))
		{
			double RTimeLMax = oldRTime + (pCurNode->left->data.mVTimeMax - oldVTime) * oldSumWeight - pCurNode->mDeltaRTime;

			if (NewRTime < RTimeLMax) //! locate in left subtree
				pCurNode = pCurNode->left;
			else//! locate in the right subtree
			{
				oldSumWeight += pCurNode->left->data.mDeltaWeight;
				pCurNode = pCurNode->right;
				oldVTime = pCurNode->left->data.mVTimeMax;
				oldRTime = RTimeLMax;
			} 
		}

		return oldVTime + (NewRTime - oldRTime) / oldSumWeight;
	}
	void Append(double curVTime,double newVTime,double newDeltaWeight)
	{
		DataField data(newVTime,newDeltaWeight);
		mpBalancedTree->insert(data);
	}

};