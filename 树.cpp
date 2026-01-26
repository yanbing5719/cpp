#include <iostream>
using namespace std;
struct TreeNode{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x):val(x),left(nullptr),right(nullptr){}
};
//前序遍历
void front(TreeNode *root){
    if(!root)return ;
    cout<<root->val<<' ';//根
    front(root->left); //左
    front(root->right); //右
}
//中序遍历
void middle(TreeNode *root){
    if(!root)return ;
    middle(root->left);
    cout<<root->val<<' ';
    middle(root->right);
}
//后序遍历
void behind(TreeNode *root){
    if(!root) return ;
    behind(root->left);
    behind(root->right);
    cout<<root->val<<' ';
}
//求树的最大深度
int maxDepth(TreeNode* root) {
       if(root==nullptr)return 0;
    int left=maxDepth(root->left);
    int right=maxDepth(root->right);
    return max(left,right)+1;
    }
//求树的结点
int nodes(TreeNode *root){
    if(!root) return 0;
    int left=nodes(root->left);
    int right=nodes(root->right);
    return (left+right+1);
}
//判断两棵树是否相同
bool issame(TreeNode *p,TreeNode *q){
    if(!p&&!q)return true;
    if(!p||!q)return false;
    return issame(p->left,q->left)&&issame(p->right,q->right);
    
}
//BTS中的搜索
TreeNode* searchBST(TreeNode* root, int val) {
        if(root==nullptr||root->val==val){
            return root;
        }
        if(root->val>val){
           return searchBST(root->left,val);
        }else{
            return searchBST(root->right,val);
        }
    }
//BTS中插入
 TreeNode* insertIntoBST(TreeNode* root, int val) {
        if(!root){
            return new TreeNode(val);
        }
        if(val<root->val){
         root->left=insertIntoBST(root->left,val);
        }else{
            root->right=insertIntoBST(root->right,val);
        }
        return root;
    }
    //BST删除结点
TreeNode *deletenode(TreeNode*root,int key){
   if(!root)return nullptr;
   if(key<root->val){
    deletenode(root->left,key);
   }else if(key>root->val){
    deletenode(root->right,key);
   }else{
    //只有两个或者一个孩子
    if(root->left==nullptr){
        TreeNode * rightroot=root->right;
        delete(root);
        return rightroot;
    }
   }if(root->right=nullptr){
        TreeNode * leftroot=root->left;
        delete(root);
        return leftroot;
    }else{
    //有两个孩子
    TreeNode *objroot=root->right;
    while(objroot->left!=nullptr){
        objroot=objroot->left;
    }
    root->val=objroot->val;
    root->right=deletenode(root->right,objroot->val);
    return root;
}
}
int main(){
    TreeNode *root=new TreeNode (1);
    root->left=new TreeNode(2);
    root->right=new TreeNode(3);
    root->left->left=new TreeNode(4);
    root->left->right=new TreeNode(5);
    root->right->left=new TreeNode(6);
    root->right->right=new TreeNode(7);
    front(root);
    cout<<endl;
    middle(root);
    cout<<endl;
    behind(root);
    cout<<endl;
    cout<<maxDepth(root)<<endl;
    cout<<nodes(root)<<endl;
}