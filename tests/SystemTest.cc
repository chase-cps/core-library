#include "representation/System.hh"
#include "representation/Component.hh"
#include "representation/Contract.hh"
#include <gtest/gtest.h>
#include <string>

using namespace chase;

TEST(SystemTest, Constructor) {
  System s("TestSystem");
  EXPECT_EQ(s.getName()->getString(), "TestSystem");
}

TEST(SystemTest, AddContract) {
  System s("TestSystem");
  auto c = new Contract("TestContract");
  s.addContract(c);

  auto contracts = s.getContractsSet();
  EXPECT_EQ(contracts.size(), 1);
  EXPECT_EQ(*contracts.begin(), c);
}

TEST(SystemTest, AddComponent) {
  System s("TestSystem");
  auto comp = new ComponentDefinition("TestCompDef");
  auto c = new Component(comp, "TestComp");
  s.addComponent(c);

  auto components = s.getComponentsSet();
  EXPECT_EQ(components.size(), 1);
  EXPECT_EQ(*components.begin(), c);
}
