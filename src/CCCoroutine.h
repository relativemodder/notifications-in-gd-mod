#ifndef __CCCOROUTINE_H__
#define __CCCOROUTINE_H__

#include <gd.h>
#include <experimental/coroutine>
#include <memory>
#include <utility>

NS_CC_BEGIN

#ifndef CC_CONSTRUCTOR_ACCESS
    #ifdef CC_ENABLE_SCRIPT_BINDING
        #define CC_CONSTRUCTOR_ACCESS public
    #else
        #define CC_CONSTRUCTOR_ACCESS protected
    #endif
#endif

class CCCoroutine
{
public:
    class promise_type;
    using handle = std::experimental::coroutine_handle<promise_type>;
    class promise_type
    {
    public:
        ~promise_type() { CC_SAFE_RELEASE(currentAction_); }

        cocos2d::CCAction *currentAction() const noexcept { return currentAction_; }
        auto final_suspend() const noexcept { return std::experimental::suspend_always {}; }
        auto get_return_object() noexcept { return CCCoroutine {handle::from_promise(*this)}; }
        auto initial_suspend() const noexcept { return std::experimental::suspend_always {}; }
        auto yield_value(cocos2d::CCAction*action)
        {
            CC_SAFE_RELEASE(currentAction_);
            currentAction_ = action;
            CC_SAFE_RETAIN(currentAction_);
            return std::experimental::suspend_always {};
        }

    private:
        cocos2d::CCAction* currentAction_ {nullptr};
    };

    CCCoroutine() = default;
    ~CCCoroutine()
    {
        if (handle_) {
            handle_.destroy();
        }
    }
    CCCoroutine(const CCCoroutine &) = delete;
    CCCoroutine &operator =(const CCCoroutine &) = delete;
    CCCoroutine(CCCoroutine &&rhs) noexcept
        : handle_ {rhs.handle_}
    {
        rhs.handle_ = nullptr;
    }
    CCCoroutine &operator =(CCCoroutine &&rhs) noexcept
    {
        if (this != std::addressof(rhs)) {
            handle_ = rhs.handle_;
            rhs.handle_ = nullptr;
        }
        return *this;
    }

    cocos2d::CCAction*currentAction() const noexcept { return handle_.promise().currentAction(); }
    bool isDone() const { return handle_ && handle_.done(); }
    bool moveNext() const { return handle_ ? (handle_.resume(), !handle_.done()) : false; }

private:
    CCCoroutine(handle h) noexcept : handle_ {h} {}

    handle handle_;
};

class CoroutineAction : public cocos2d::CCAction
{
public:
    static CoroutineAction *create(CCCoroutine &&coroutine)
    {
        CoroutineAction *ret = new (std::nothrow) CoroutineAction();
        if (ret && ret->initWithCoroutine(std::forward<CCCoroutine>(coroutine)))
        {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    virtual bool isDone() const
    {
        const auto action = coroutine_.currentAction();
        if (action && !action->isDone()) {
            return false;
        }
        if (!coroutine_.isDone()) {
            return false;
        }
        return true;
    }
    virtual void step(float dt) override
    {
        auto action = coroutine_.currentAction();
        if (action && !action->isDone()) {
            action->step(dt);
            return;
        }
        coroutine_.moveNext();
    }


CC_CONSTRUCTOR_ACCESS:
    CoroutineAction() = default;
    bool initWithCoroutine(CCCoroutine &&coroutine) noexcept
    {
        coroutine_ = std::forward<CCCoroutine>(coroutine);
        return true;
    }

private:
    //CC_DISALLOW_COPY_AND_ASSIGN(CoroutineAction);

    CCCoroutine coroutine_;
};


cocos2d::CCAction* startCoroutine(cocos2d::CCNode* node, cocos2d::CCCoroutine&& coroutine)
{
    return node->runAction(cocos2d::CoroutineAction::create(std::forward<cocos2d::CCCoroutine>(coroutine)));
}

NS_CC_END

#endif // __CCCOROUTINE_H__
