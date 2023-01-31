#include <iostream>
#include <queue>
#include <climits>
using namespace std;




struct cord{
    int x,y;
};
 

struct node{
    int x, y, dist;
    node *parent;
    node(int i,int j,int d,node *p){
        x=i;
        y=j;
        dist=d;
        parent=p;
    }
    node(): x(0),y(0),dist(0),parent(NULL){

    }
};




class BTreeNode{
    private:
        node *keys;
        int t;
        BTreeNode **C;
        int n;
        bool leaf;
    public:
        BTreeNode(int _t,bool _leaf);
        void insertNonFull(node k);
        void splitChild(int i,BTreeNode *y);
        void traverse();
        BTreeNode *search(node k);
    friend class BTree;
};


class BTree{
    private:
        BTreeNode *root;
        int t;
    public:
        BTree(int _t){
            root=NULL;
            t=_t;
        }
        void traverse(){
            if(root != NULL){
                root->traverse();
            }
        } 
        BTreeNode* search(node k){
            if(root==NULL){
                return NULL;
            }
            else{
                return root->search(k);
            }
        }

        void insert(node k);
};


BTreeNode::BTreeNode(int t1,bool leaf1){
    t=t1;
    leaf=leaf1;
    keys=new node[2*t-1];
    C=new BTreeNode *[2*t];
    n=0;
}


void BTreeNode::traverse(){
    int i;
    for(i=0;i<n;i++){
        if(leaf==false){
            C[i]->traverse();
        }
        cout<<" "<<keys[i].dist;
    }
    if(leaf==false){
        C[i]->traverse();
    }
}

BTreeNode *BTreeNode::search(node k){
    int i=0;
    while(i<n && k.dist>keys[i].dist){
        i++;
    }
    if(keys[i].x==k.x && keys[i].y==k.y){
        return this;
    }
    if(leaf==true){
        return NULL;
    }
    return C[i]->search(k);
}



void BTree::insert(node k){
    if(root==NULL){
        root=new BTreeNode(t,true);
        root->keys[0]=k;
        root->n=1;
    }
    else{
        if(root->n==2*t-1){
            BTreeNode *s=new BTreeNode(t,false);
            s->C[0]=root;
            s->splitChild(0,root);
            int i=0; 
            if(s->keys[0].dist<k.dist){
                i++;
            }
            s->C[i]->insertNonFull(k);
            root=s;
        }
        else{
            root->insertNonFull(k);
        }
    } 
}



void BTreeNode::insertNonFull(node k){
    int i=n-1;
    if(leaf==true){
        while(i>=0 && keys[i].dist>k.dist){ 
            keys[i+1]=keys[i];
            i--;
        }
        keys[i+1]=k;
        n=n+1;
    }
    else{
        while(i>=0 && keys[i].dist>k.dist){
            i--;
        }
        if(C[i+1]->n==2*t-1){
            splitChild(i+1,C[i+1]);
            if(keys[i+1].dist<k.dist){
                i++;
            }
        }
        C[i+1]->insertNonFull(k);
    }
}



void BTreeNode::splitChild(int i,BTreeNode *y){
    BTreeNode *z=new BTreeNode(y->t,y->leaf);
    z->n=t-1;
    for(int j=0;j<t-1;j++){
        z->keys[j]=y->keys[j+t];
    }

    if(y->leaf==false){ 
        for(int j=0;j<t;j++){
            z->C[j]=y->C[j+t];
        }
    }
    y->n=t-1;
    for(int j=n;j>=i+1;j--){
        C[j+1]=C[j];
    }
    C[i+1]=z;
    for(int j=n-1; j>=i;j--){
        keys[j+1]=keys[j];
    }

    keys[i]=y->keys[t-1];
    n=n+1;
}



 

bool valid(int **mat, int i, int j,int row,int col){
    return (i >= 0) && (i < row) && (j >= 0) && (j < col) && mat[i][j] ;
}



void bfs(int **mat,int row,int col,cord start,cord end){
    int rows[]={-1,0,0,1};
    int cols[]={0,-1,1,0};
    BTree t(3);
    queue <node *> q;
    node *n=new node(start.x,start.y,0,NULL);
    q.push(n);
    t.insert(*n);
    int min_dist=INT_MAX;
    node *m;
    while(!q.empty()){
        m=q.front();
        q.pop();
        int i=m->x;
        int j=m->y;
        int dist=m->dist;
        if(i==end.x  &&  j==end.y){
            min_dist=dist;
            break;
        }
        for(int k=0;k<4;k++){
            node *n=new node(i+rows[k],j+cols[k],dist+1,m);
            if ( valid(mat,i+rows[k],j+cols[k],row,col) && !t.search(*n) ){
                t.insert(*n);
                q.push(n);
            }
        }
    }
    if (min_dist!=INT_MAX){
        while(m!=NULL){
            mat[m->x][m->y]=-1;
            m=m->parent;
        }
    }
    else{
        cout<<"no such path";
    }
}



int main(){
    int row,col;
    cin>>row>>col;
    int **matrix=new int*[row];
    for(int i=0;i<row;i++){
        matrix[i]=new int[col];
    }
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            cin>>matrix[i][j];
        }
    }
    cord start;
    for(int i=0;i<col;i++){
        if(matrix[0][i]==1){
            start.x=0;
            start.y=i;
        }
    }
    cord end;
    for(int i=0;i<col;i++){
      if(matrix[row-1][i]==1){
        end.x=row-1;
        end.y=i;
      }
    }

    bfs(matrix,row,col,start,end);
    
    cout<<endl<<endl<<endl;
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            cout<<matrix[i][j]<<" ";
        }
        cout<<endl;
    }
    return 0;
}