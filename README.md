## SimpleTest
一个简单的C++测试框架

### Version: 1.0.1

### Integration
将位于`include`目录中的 [xf_simple_test.h](include/xf_simple_test.h) 头文件拷贝到自己的项目中即可。

### Usage
* 包含头文件:
```C++
#include "xf_simple_test.h"
```
* 定义单元测试函数:
  ```c++
    _xfTest(test_1)
    {
        // ...
        _xfExpect(1 == 2);
        _xfExpect(1 < 2);
        _xfExpect(1 > 2);
    }
  ```
* 在main函数中执行测试：
  ```c++
    int main()
    {
	    /*
		 * 建议将测试结果作为测试程序的返回码
		 * 如果测试成功返回码为0，否则为失败的测试用例数量。
		 */
        return xf::test::Test();
        // return xf::test::Test({"test_1"});
    }
  ```
* 示例输出：
  ```text
  --> test failed in test_1:
  at ../example/example.cpp:4

  --> test failed in test_1:
  at ../example/example.cpp:6

  ==> Ran 3 tests from 1 test case: 1 successes, 2 failures, spend 12 ms.
  ==> Test Result: FAIL.
  ```
  更多示例参见 [example.cpp](example/example.cpp)

### Design
该测试框架设计简单，适用于要求不高的简单单元测试，仅通过宏定义来描述测试过程。

* **_xfTest:** 定义一个单元测试函数，此宏可以在全局或者命名空间中使用，但不可以在类中使用。
* **_xfExpect，_xfAssert:** 对结果进行断言，区别在于 `_xfAssert` 断言失败则函数终止，而 `_xfExpect` 即使失败函数依然继续执行。
* **xf::test::Test:** 执行由宏 `_xfTest` 定义的单元测试函数，可以在 `xf::test::Test` 中指定参数来选择要执行的测试函数，不指定时执行所有测试。
