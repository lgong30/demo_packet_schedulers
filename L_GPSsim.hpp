

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
	double RTime2VTime(double NewRTime)
	{
		//!
		double oldVTime = mOldVTime,
			   oldRTime = mOldRTime,
			   oldSumWeight = mSumWeight;

		Node *pCurNode = mpBalancedTree.GetRoot();

		while (!mpBalancedTree->IsLeaf(pCurNode))
		{
			double RTimeLMax = oldRTime + (pCurNode->left->mVTimeMax - oldVTime) * oldSumWeight - pCurNode->mDeltaRTime;

			if (NewRTime < RTimeLMax)
				pCurNode = pCurNode->left;
			else
			{
				pCurNode = pCurNode->right;
				oldVTime = pCurNode->left->mVTimeMax;
				oldRTime = RTimeLMax;
				oldSumWeight += pCurNode->left->mDeltaWeight;
			} 
		}

		return oldVTime + (NewRTime - oldRTime) / oldSumWeight;
	}

};