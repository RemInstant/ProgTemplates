#include <iostream>
#include <vector>
#include <limits>

using namespace std;
using ll = long long;
using ull = unsigned long long;

class TwoDTree
{
    
private:

    struct TreeNode
    {
        ll x1, x2;
        size_t subtree_count;
        TreeNode *left;
        TreeNode *right;
    };
    
    TreeNode *root;
    
public:
    
    TwoDTree(vector<pair<ll, ll>> points)
    {
        root = build_node(points, 0);
    }
    
    size_t count_points_in(ll x1, ll y1, ll x2, ll y2)
    {
        ll mn = numeric_limits<ll>::min();
        ll mx = numeric_limits<ll>::max();
        
        return count_points_in(x1, y1, x2, y2, root, 0, mn, mn, mx, mx);
    }

private:

    TreeNode *build_node(vector<pair<ll, ll>> &points, size_t height)
    {
        if (points.size() == 0)
        {
            return nullptr;
        }
        
        if (points.size() == 1)
        {
            TreeNode *leaf = new TreeNode();
            leaf->x1 = points[0].first;
            leaf->x2 = points[0].second;
            leaf->subtree_count = 1;
            leaf->left = leaf->right = nullptr;
            
            return leaf;
        }
        
        vector<pair<ll,ll>> to_left, to_right;
        pair<ll,ll> median;
        
        to_left.reserve(points.size() / 2);
        to_right.reserve(points.size() / 2 + 1);
        
        if (height % 2 == 0)
        {
            sort(points.begin(), points.end(),
                [](pair<ll, ll> a, pair<ll, ll> b){ return a.first < b.first; });
            
            median = points[points.size() / 2];
            
            for (size_t i = 0; i < points.size(); ++i)
            {
                if (i == points.size() / 2) continue;
                if (points[i].first < median.first)
                {
                    to_left.push_back(move(points[i]));
                }
                else
                {
                    to_right.push_back(move(points[i]));
                }
            }
        }
        else
        {
            sort(points.begin(), points.end(),
                [](pair<ll, ll> a, pair<ll, ll> b){ return a.second < b.second; });
            
            median = points[points.size() / 2];
            
            for (size_t i = 0; i < points.size(); ++i)
            {
                if (i == points.size() / 2) continue;
                if (points[i].second < median.second)
                {
                    to_left.push_back(move(points[i]));
                }
                else
                {
                    to_right.push_back(move(points[i]));
                }
            }
        }
        
        points.clear();
        
        TreeNode *node = new TreeNode();
        node->x1 = median.first;
        node->x2 = median.second;
        node->left = build_node(to_left, height + 1);
        node->right = build_node(to_right, height + 1);
        node->subtree_count = get_subtree_count(node->left) + get_subtree_count(node->right) + 1;
        
        return node;
    }
    
    size_t count_points_in(ll x1, ll y1, ll x2, ll y2, TreeNode *node, size_t height,
            ll qx1, ll qy1, ll qx2, ll qy2)
    {
        if (node == nullptr)
        {
            return 0;
        }
        
        if (node->left == node->right) // nullptr
        {
            return included(x1, y1, x2, y2, node->x1, node->x2) ? 1 : 0;
        }
        
        if (x1 < qx1 && qx2 < x2 && y1 < qy1 && qy2 < y2)
        {
            return node->subtree_count;
        }
        
        if (height % 2 == 0)
        {
            ll split = node->x1;
            
            if (x2 <= split)
            {
                return count_points_in(x1, y1, x2, y2, node->left, height + 1,
                        qx1, qy1, split, qy2);
            }
            
            if (x1 >= split)
            {
                return count_points_in(x1, y1, x2, y2, node->right, height + 1,
                        split, qy1, qx2, qy2);
            }
            
            size_t left_cnt = count_points_in(x1, y1, x2, y2, node->left, height + 1,
                    qx1, qy1, split, qy2);
            size_t right_cnt = count_points_in(x1, y1, x2, y2, node->right, height + 1,
                    split, qy1, qx2, qy2);
            
            return left_cnt + right_cnt +
                    (included(x1, y1, x2, y2, node->x1, node->x2) ? 1 : 0);
        }
        else
        {
            ll split = node->x2;
            
            if (y2 <= split)
            {
                return count_points_in(x1, y1, x2, y2, node->left, height + 1,
                        qx1, qy1, qx2, split);
            }
            
            if (y1 >= split)
            {
                return count_points_in(x1, y1, x2, y2, node->right, height + 1,
                        qx1, split, qx2, qy2);
            }
            
            size_t left_cnt = count_points_in(x1, y1, x2, y2, node->left, height + 1,
                    qx1, qy1, qx2, split);
            size_t right_cnt = count_points_in(x1, y1, x2, y2, node->right, height + 1,
                    qx1, split, qx2, qy2);
            
            return left_cnt + right_cnt +
                    (included(x1, y1, x2, y2, node->x1, node->x2) ? 1 : 0);
        }
    }

private:
    
    size_t get_subtree_count(TreeNode *node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        
        return node->subtree_count;
    }
    
    bool included(ll x1, ll y1, ll x2, ll y2, ll x, ll y)
    {
        return (x1 < x && x < x2) && (y1 < y && y < y2);
    }

    
};


int main()
{
    vector<pair<ll, ll>> points =
    {
        {1, 1}, {1, -1}, {-1, -1}, {-1, 1}
    };
    
    TwoDTree tree(points);
    
    cout << tree.count_points_in(-2, -2, 2, 2) << endl;
    cout << tree.count_points_in( 0, -2, 2, 2) << endl;
    cout << tree.count_points_in(-2,  0, 2, 2) << endl;
    cout << tree.count_points_in(-2, -2, 0, 2) << endl;
    cout << tree.count_points_in(-2, -2, 2, 0) << endl;
    cout << tree.count_points_in( 0,  0, 2, 2) << endl;
    cout << tree.count_points_in(-2, -2, 0, 0) << endl;
    cout << tree.count_points_in( 0, -2, 2, 0) << endl;
    cout << tree.count_points_in(-2,  0, 0, 2) << endl;
}