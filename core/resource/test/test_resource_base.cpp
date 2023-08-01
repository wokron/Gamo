#include "gtest/gtest.h"
#include "resource.h"
#include <vector>
#include <algorithm>

using namespace gamo;

class MyResource;

class MyResourceSet : public IResourceSet<MyResource>
{
public:
    std::vector<MyResource *> members;
    void Remove(MyResource *item) override;
};

class MyResource : public IResource, public IResourceSet<MyResource>
{
public:
    MyResourceSet *belong = nullptr;
    bool is_freed = false;
    std::vector<MyResource *> sub_resources;
    MyResource *sup_resource = nullptr;

    void Destroy() override
    {
        if (sup_resource != nullptr)
            sup_resource->Remove(this);
        else
            belong->Remove(this);
    }

    void Deref() override
    {
        RegisterHandleMemFree();
        RemoveAll();
    }

    OVERRIDE_HANDLE_MEM_FREE(MyResource)
    {
        is_freed = !is_freed;
        UnregisterHandleMemFree();
    }

    void Remove(MyResource *item) override
    {
        auto find = std::find(sub_resources.begin(), sub_resources.end(), item);

        if (find == sub_resources.end())
            return;

        sub_resources.erase(find);

        item->Deref();
    }

    void RemoveAll() override 
    {
        for (auto item : sub_resources)
        {
            Remove(item);
        }
    }
};

void MyResourceSet::Remove(MyResource *item)
{
    auto find = std::find(members.begin(), members.end(), item);

    if (find == members.end())
        return;

    members.erase(find);

    item->Deref();
}

TEST(TestResource, test_destroy)
{
    MyResourceSet resource_set;
    MyResource *resource_arr[10];
    for (int i = 0; i < 10; i++)
    {
        resource_arr[i] = new MyResource();
        resource_arr[i]->belong = &resource_set;
        resource_set.members.push_back(resource_arr[i]);
    }

    resource_arr[0]->Destroy();
    resource_arr[2]->Destroy();
    resource_arr[3]->Destroy();

    for (int i = 0; i < 10; i++)
    {
        ASSERT_FALSE(resource_arr[i]->is_freed);
    }

    EventDispatcher::GetInstance()->Dispatch(EVENT_MEM_FREE, nullptr);
    ASSERT_TRUE(resource_arr[0]->is_freed);
    ASSERT_TRUE(resource_arr[2]->is_freed);
    ASSERT_TRUE(resource_arr[3]->is_freed);

    // only free once, so true as well
    EventDispatcher::GetInstance()->Dispatch(EVENT_MEM_FREE, nullptr);
    ASSERT_TRUE(resource_arr[0]->is_freed);
    ASSERT_TRUE(resource_arr[2]->is_freed);
    ASSERT_TRUE(resource_arr[3]->is_freed);

    ASSERT_EQ(std::count(resource_set.members.begin(), resource_set.members.end(), resource_arr[0]), 0);
    ASSERT_EQ(std::count(resource_set.members.begin(), resource_set.members.end(), resource_arr[2]), 0);
    ASSERT_EQ(std::count(resource_set.members.begin(), resource_set.members.end(), resource_arr[3]), 0);

    ASSERT_NE(std::count(resource_set.members.begin(), resource_set.members.end(), resource_arr[1]), 0);
    ASSERT_NE(std::count(resource_set.members.begin(), resource_set.members.end(), resource_arr[4]), 0);
    ASSERT_NE(std::count(resource_set.members.begin(), resource_set.members.end(), resource_arr[5]), 0);
}

TEST(TestResource, test_resource_tree)
{
    /// build tree like this:
    // set
    // ├── r1
    // │   ├── r3
    // │   │   └── r4
    // │   └── r5
    // └── r2
    MyResourceSet resource_set;
    auto r1 = new MyResource();
    r1->belong = &resource_set;
    resource_set.members.push_back(r1);
    
    auto r2 = new MyResource();
    r2->belong = &resource_set;
    resource_set.members.push_back(r2);

    auto r3 = new MyResource();
    r3->belong = &resource_set;
    resource_set.members.push_back(r3);
    r3->sup_resource = r1;
    r1->sub_resources.push_back(r3);

    auto r4 = new MyResource();
    r4->belong = &resource_set;
    resource_set.members.push_back(r4);
    r4->sup_resource = r3;
    r3->sub_resources.push_back(r4);

    auto r5 = new MyResource();
    r5->belong = &resource_set;
    resource_set.members.push_back(r5);
    r5->sup_resource = r1;
    r1->sub_resources.push_back(r5);

    // set
    // ├── r1
    // │   └── r3
    // │        └── r4 
    // └── r2
    r5->Destroy();
    EventDispatcher::GetInstance()->Dispatch(EVENT_MEM_FREE, nullptr);
    ASSERT_TRUE(r5->is_freed);

    // set
    // ├── r1
    // └── r2
    r3->Destroy();
    EventDispatcher::GetInstance()->Dispatch(EVENT_MEM_FREE, nullptr);
    ASSERT_TRUE(r3->is_freed);
    ASSERT_TRUE(r4->is_freed);
    ASSERT_TRUE(r1->sub_resources.empty());
    ASSERT_TRUE(r2->sub_resources.empty());

}
