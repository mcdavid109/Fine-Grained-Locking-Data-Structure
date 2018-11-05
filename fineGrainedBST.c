//Fine Grained Locking BST

struct Node{
	Node* left , right;
	int val;
	Lock lock;
};

struct Tree{
	Node* root;
	Lock lock;
}

Node* createNode(int val){
	Node* newNode = new Node();
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->val = val;
	return newNode;
}

Node* insertLocking(Tree* tree , Node* root , int val , Node* parent){
	if(root){
		lock(root->lock);
		if(parent) unlock(parent->lock);
		if(root->val == val){
			unlock(root->lock);
			return root;
		}
		else if(root->val > val){
			if(root->left){
				return insertLocking(tree , root->left , val , root);
			}
			else{
				root->left = createNode(val);
				unlock(root->lock);
				return root->left;
			}
		}
		else{
			if(root->right){
				return insertLocking(tree , root->right , val , root)
			}
			else{
				root->right = createNode(val);
				unlock(root->lock);
				return root->right;
			}
		}
	}
	else{
		lock(tree->lock);
		root = createNode(val);
		unlock(tree->lock);
	}
}

Node* insert(Tree* tree , int val){
	return insertLocking(tree , tree->root , val , NULL);
}

Node* deleteLocking(Tree* tree , Node* root , int val , Node* parent){
	if(root){
		lock(root->lock);
		if(parent) unlock(parent->lock);
		if(root->val == val){
			if(!root->left && !root->right){
				Node* tmp = root;
				root = NULL;
				unlock(tmp->lock);
				delete tmp;
				return root;
			}
			else if(!root->left && root->right){
				Node* tmp = root;
				root = root->right;
				unlock(tmp->lock);
				delete tmp;
				return root;
			}
			else if(root->left && !root->right){
				Node* tmp = root; 
				root = root->left;
				unlock(tmp->lock);
				delete tmp;
				return root;
			}
			else{
				Node* tmp = findMin(root->right , root);
				swap(root->val , tmp->val);
				root->right =  deleteLocking(tree , root->right , tmp->val , root);
			}
		}
		else if(root->val > val){
			root->left = deleteLocking(tree , root->left , val , root);
		}
		else{
			root->right = deleteLocking(tree , root->right , val , root);
		}
	}
	else{
		return NULL;
	}
}

Node* findMin(Node* root , Node* parent){
	if(root){
		lock(root->lock);
		if(parent) unlock(parent->lock);
		if(parent->left){
			return findMin(root->left , root);
		}
		else{
			unlock(root->lock);
			return root;
		}
	}
	else{
		return NULL;
	}
}