#include <gtest/gtest.h>

#include "../src/node.hpp"

using namespace strpercpp;

TEST(TestPriorityQueue, NodeQueue) {
  node_ptr_queue pq;

  node_ptr n1(new Node());
  n1->path_score = 5;
  pq.push(n1);

  node_ptr n2(new Node());
  n2->path_score = 4;
  pq.push(n2);

  node_ptr n3(new Node());
  n3->path_score = 2;
  pq.push(n3);

  node_ptr n4(new Node());
  n4->path_score = 7;
  pq.push(n4);

//  node_ptr n1_ = pq.top();
//  pq.pop();
//  EXPECT_EQ(2., n1_->path_score);

//  node_ptr n2_ = pq.top();
//  pq.pop();
//  EXPECT_EQ(4., n2_->path_score);

//  node_ptr n3_ = pq.top();
//  pq.pop();
//  EXPECT_EQ(5., n3_->path_score);

//  node_ptr n4_ = pq.top();
//  pq.pop();
//  EXPECT_EQ(7., n4_->path_score);


  node_ptr n1_ = pq.top();
  pq.pop();
  EXPECT_EQ(7., n1_->path_score);

  node_ptr n2_ = pq.top();
  pq.pop();
  EXPECT_EQ(5., n2_->path_score);

  node_ptr n3_ = pq.top();
  pq.pop();
  EXPECT_EQ(4., n3_->path_score);

  node_ptr n4_ = pq.top();
  pq.pop();
  EXPECT_EQ(2., n4_->path_score);

  EXPECT_TRUE(pq.empty());

}


