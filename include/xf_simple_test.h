#pragma once

#include <map>
#include <vector>
#include <chrono>
#include <functional>
#include <iostream>

namespace xf::test
{

    const char* version() { return "1.0.0"; }

    class TestInfo final
    {
    private:

        using _func_type = std::function<void(const std::string&)>;
        using _func_map_t = std::map<std::string, _func_type>;

        _func_map_t _FuncMap;
        std::pair<unsigned int, unsigned int> _result{ 0, 0 };

        TestInfo() = default;
        TestInfo(const TestInfo&) = delete;
        TestInfo& operator = (const TestInfo&) = delete;

        bool _run(const std::string& name, const _func_type& f) const
        {
            f(name);
            return true;
        }

    public:

        auto Size() const { return _FuncMap.size(); }

        const auto& Result() const { return _result; }

        void Run() const
        {
            for (const auto& [name, func] : _FuncMap)
                _run(name, func);
        }

        bool Run(const std::string& key) const
        {
            if (auto iter = _FuncMap.find(key); iter != _FuncMap.end())
            {
                _run(iter->first, iter->second);
                return true;
            }

            return false;
        }

        bool Add(const std::string& key, _func_type func)
        {
            auto iter = _FuncMap.emplace(key, func);
            return iter.second;
        }

        void Counting(bool value)
        {
            if (value) ++_result.first; else ++_result.second;
        }

        static TestInfo& Instance()
        {
            static TestInfo _Inst_;
            return (_Inst_);
        }

    };  // class TestInfo

    void Show(unsigned int n, unsigned int success, unsigned int failed, long long ms)
    {
        std::cout << std::endl;
        std::cout << "==> Ran " << (success + failed) << " tests from " << n << " test case: "
            << success << " successes, " << failed << " failures, spend " << ms << " ms." << std::endl;
        std::cout << "==> Test Result: " << (0 == failed ? "SUCCESS." : "FAIL.") << std::endl;
    }

    bool Assert(bool result, const std::string& name, const std::string& file, unsigned int line)
    {
        TestInfo::Instance().Counting(result);
        if (!result)
        {
            std::cout << "--> test failed in " << name << ":" << std::endl;
            std::cout << "at " << file << ":" << line << std::endl << std::endl;
        }

        return result;
    }

    void Test()
    {
        auto t1 = std::chrono::system_clock::now();
        TestInfo::Instance().Run();
        auto t2 = std::chrono::system_clock::now();

        const auto& [success, failed] = TestInfo::Instance().Result();
        Show(TestInfo::Instance().Size(), success, failed, std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());
    }

    void Test(const std::vector<std::string>& names)
    {
        auto t1 = std::chrono::system_clock::now();
        for (const auto& name : names)
            TestInfo::Instance().Run(name);
        auto t2 = std::chrono::system_clock::now();

        const auto& [success, failed] = TestInfo::Instance().Result();
        Show(names.size(), success, failed, std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());
    }

}   // namespace xf::test

#define _xfConcatenateName(a, b) a##b
#define _xfDeclareTestFunc(key) void _xfConcatenateName(_test_function_name_, key)(const std::string& name)

#define _xfAddTest(key, func) xf::test::TestInfo::Instance().Add(key, func)

#define _xfTest(key) _xfDeclareTestFunc(key); \
auto _xfConcatenateName(_test_variable_name_, key) = \
_xfAddTest(#key, _xfConcatenateName(_test_function_name_, key)); \
_xfDeclareTestFunc(key)

#define _xfExpect(expr) xf::test::Assert(expr, name, __FILE__, __LINE__)
#define _xfAssert(expr) if (!xf::test::Assert(expr, name, __FILE__, __LINE__)) return