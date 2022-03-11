
#ifndef BUTIPTR_H
#define BUTIPTR_H
#include<string>
#include<type_traits>
namespace std {
template <typename T>
inline std::string to_string(const T&)noexcept {
	return "このクラスはto_string()に対応していません";
}
#ifdef __vm_value_h__
template<typename T>
inline std::string to_string(const ButiScript::Type_hasMember<T>& arg_v);
#endif // __vm_value_h__

}
namespace ButiEngine {

template <class T, class = void>
struct _Can_enable_value: std::false_type {}; // detect unambiguous and accessible inheritance from enable_shared_from_this

template <class T>
struct _Can_enable_value<T, std::void_t<typename T::_Evft_type>>
	: std::is_convertible<std::remove_cv_t<T>*, typename T::_Evft_type*>::type {};
namespace SmartPtrDetail {
namespace RefferenceCounter {
class RefferenceCounter;
}
}

template<typename T,typename RefferenceObject= SmartPtrDetail::RefferenceCounter::RefferenceCounter>
class Value_ptr;

/// <summary>
/// Value_ptr<void>を保存するために経由するインターフェース
/// </summary>
class IValuePtrRestoreObject {
public:
	virtual void RestoreValue(ButiEngine::Value_ptr<void>&)const = 0;
	template<class Archive>	void serialize(Archive& archive){}
};


template<typename T>
constexpr bool False(){ return false; }

namespace SmartPtrDetail {
template <typename T>
T* CloneValue(const T* arg_p_v) {
	if constexpr (std::is_copy_constructible_v<T>) {
		return new T(*arg_p_v);
	}
	else if constexpr(std::is_copy_assignable_v<T>&&std::is_default_constructible_v<T>) {
		auto output = new T();
		*output = *arg_p_v;
		return output;
	}
	assert(0 && "コピーが不可能な型をコピーしようとしています");
	return nullptr;
}

class MemberPtrNotify {};
static MemberPtrNotify MemberNotify;
namespace TypeStorage {
struct has_ShowGUI_StrArg_impl {
	template <typename T>
	static auto check(T&& x)->decltype(x.ShowGUI("TestLabel"), std::true_type{});

	template <typename T>
	static auto check(...)->std::false_type;
};
struct has_ShowGUI_impl {
	template <typename T>
	static auto check(T&& x)->decltype(x.ShowGUI(), std::true_type{});

	template <typename T>
	static auto check(...)->std::false_type;
};
template <typename T>
class has_ShowGUI_StrArg : public decltype(has_ShowGUI_StrArg_impl::check<T>(std::declval<T>())) {};
template <typename T>
class has_ShowGUI : public decltype(has_ShowGUI_impl::check<T>(std::declval<T>())) {};

template <typename T>
constexpr bool has_ShowGUI_StrArg_v = has_ShowGUI_StrArg<T>();
template <typename T>
constexpr bool has_ShowGUI_v = has_ShowGUI<T>();

template<typename T>struct type_to_ref_impl { using reftype = T&; };
template <> struct type_to_ref_impl<void> { using reftype = void; };

template<typename T>struct type_to_constRef_impl { using reftype =const T&; };
template <> struct type_to_constRef_impl <void> { using reftype = void; };

template <typename T> using type_to_ref =typename type_to_ref_impl<T>::reftype;
template <typename T> using type_to_constRef = typename type_to_constRef_impl<T>::reftype;

#ifdef BUTIGUI_H
struct InputCheck {
	template <class T>
	auto operator()(T&& arg_check) -> decltype(GUI::Input("", arg_check));
};
#endif
template<typename T>
inline T& Through(T& arg_ref_v) { return arg_ref_v; }

template <typename T>
class TypeStorage;
template <typename T>
class MemberTypeStorage;
/// <summary>
/// インスタンスが生成された際の型を記憶し、適切なデストラクタ呼び出し、コピー、String変換、GUI呼び出しを行う
/// </summary>
class ITypeStorage {
public:
	virtual void Destroy() = 0;
	virtual void Dispose() = 0;
	virtual ITypeStorage* Clone() const= 0;
	virtual ITypeStorage* CreateSameTypeStorage(void * arg_src)const = 0;
	virtual std::string ToString() const= 0;
	virtual void* ptr() = 0;
	virtual const void* ptr() const = 0;
	template <typename OwnType,typename CastType>
	inline ITypeStorage* StaticCast()const;
	template<std::int32_t size>
	inline void Write(const void* arg_src) {
		memcpy_s(ptr(), size, arg_src, size);
	}
	virtual void Write(const void* arg_src) = 0;
	virtual bool ShowGUI(const std::string& arg_label) = 0;
	virtual inline void GetRestoreObject(Value_ptr<IValuePtrRestoreObject>& arg_ref_vlp) const = 0;
protected:
	template<typename T>
	inline bool ShowGUI_(const std::string& arg_label, T& arg_value) {
#ifdef BUTIGUI_H
		if constexpr (std::is_invocable_v<InputCheck, T>)
		{
			return GUI::Input(arg_label, arg_value);
		}
		else if constexpr (has_ShowGUI_v<T>)
		{
			arg_value.ShowGUI();
		}
		else if constexpr (has_ShowGUI_StrArg_v<T>)
		{
			return arg_value.ShowGUI(arg_label);
		}
		else {
			GUI::Text(arg_label + u8":対応していない型です");
		}
#endif // BUTIGUI_H
		return false;
	}
};
template <typename T>
class TypeStorage : public ITypeStorage {
public:
	using this_type = TypeStorage<T>;
	TypeStorage(T* arg_p) :p_value(arg_p) {}
	inline void Dispose()override { delete p_value; }
	inline void Destroy()override { delete this; }
	//値のクローン作成(今後メモリ確保は変更)
	inline ITypeStorage* Clone()const override { 
		return new this_type(CloneValue(p_value)); 
	}
	//同じ型のTypeStorageを作成する
	inline ITypeStorage* CreateSameTypeStorage(void* arg_src)const override { return new this_type(reinterpret_cast<T*>(arg_src)); }
	inline std::string ToString()const override {
		if constexpr (std::is_same_v<std::string, std::remove_all_extents_t<T>>) {
			return *p_value;
		}
		else {
			return std::to_string(*p_value);
		}
	}
	inline  void* ptr()override { return p_value; }
	inline const void* ptr()const override { return p_value; }
	inline void Write(const void* arg_src)override {
		if constexpr (std::is_copy_assignable_v<T>)
		{
			*p_value = *reinterpret_cast<const T*>(arg_src);
		}
	}
	static this_type* get(T* arg_p) { return new this_type(arg_p); }
	inline bool ShowGUI(const std::string& arg_label) {
		return ShowGUI_(arg_label, *p_value);
	}
	inline void GetRestoreObject(Value_ptr<IValuePtrRestoreObject>& arg_ref_vlp)const override;
private:
	template<typename T> friend class MemberTypeStorage;
	this_type& operator=(const this_type&) { return *this; }
	TypeStorage(const this_type& arg_rhs) : p_value(arg_rhs.p_value) {}
	TypeStorage() = delete;
	T* p_value;
}; 
template <typename T>
class MemberTypeStorage : public ITypeStorage {
public:
	using this_type = MemberTypeStorage<T>;
	inline void Dispose()override {  }
	inline void Destroy()override { delete this; }
	//値のクローン作成(今後メモリ確保は変更)
	inline ITypeStorage* Clone()const override { 
		return new TypeStorage<T>(CloneValue(p_value));
	}
	//同じ型のTypeStorageを作成する
	inline ITypeStorage* CreateSameTypeStorage(void* arg_src)const override { return new this_type(reinterpret_cast<T*>(arg_src)); }
	inline  void* ptr()override { return p_value; }
	inline const void* ptr()const override { return p_value; }
	inline std::string ToString()const override {
		if constexpr (std::is_same_v<std::string, std::remove_all_extents_t<T>>) {
			return *p_value;
		}
		else {
			return std::to_string(*p_value);
		}
	}
	static this_type* get(T* arg_p) { return new this_type(arg_p); }
	inline void Write(const void* arg_src)override {
		*p_value = *reinterpret_cast<const T*>(arg_src);
	}
	inline bool ShowGUI(const std::string& arg_label) {
		return ShowGUI_(arg_label, *p_value);
	}
	inline void GetRestoreObject(Value_ptr<IValuePtrRestoreObject>& arg_ref_vlp)const override;
private:
	this_type& operator=(const this_type&) { return *this; }
	MemberTypeStorage(const this_type&) = delete;
	MemberTypeStorage() = delete;
	MemberTypeStorage(T* arg_p) :p_value(arg_p) {}
	T* p_value;
};

template<typename OwnType, typename CastType>
inline ITypeStorage* ITypeStorage::StaticCast() const
{
	return new TypeStorage(new CastType( static_cast<CastType>(* reinterpret_cast<const OwnType*>(ptr()))));
}

class TypeStorageSpecifier {
public:
	template<typename T> class type : public TypeStorage<T> {
	public:
		using name = TypeStorage<T>;
	};
};
class MemberTypeStorageSpecifier {
public:
	template<typename T> class type : public MemberTypeStorage<T> {
	public:
		using name = MemberTypeStorage<T>;
	};
};
}
namespace RefferenceCounter {

class RefferenceCounter {
	struct impl;
	friend class RefferenceCounter_weak;
public:
	constexpr RefferenceCounter()noexcept :p_impl(nullptr) {}
	constexpr RefferenceCounter(impl* arg_p_impl) :p_impl(arg_p_impl) {}
	RefferenceCounter(RefferenceCounter* arg_r) : p_impl(arg_r->p_impl) {}
	RefferenceCounter(const RefferenceCounter& arg_r) :p_impl(arg_r.p_impl) { if (p_impl) p_impl->inc(); }
	template<typename S, typename D>
	explicit RefferenceCounter(S* arg_p, D arg_d) :p_impl(new impl(arg_p, arg_d)) {}
	template<typename S, typename D>
	explicit RefferenceCounter(S* arg_p, D* arg_p_d) :p_impl(new impl(arg_p, arg_p_d)) {}
	~RefferenceCounter() { release(); }
	inline void release() {
		if (p_impl && !p_impl->dec()) {
			p_impl->p_typeStorage->Dispose();
			delete p_impl;
		}
	}
	inline void Write(const void* arg_src) {
		p_impl->p_typeStorage->Write(arg_src);
	}
	inline void swap(RefferenceCounter& r) { std::swap(p_impl, r.p_impl); }
	inline TypeStorage::ITypeStorage* GetTypeStorage() { return p_impl->p_typeStorage; }
	inline const TypeStorage::ITypeStorage* GetTypeStorage()const { return p_impl->p_typeStorage; }
	inline bool unique()const { return use_count() == 1; }
	inline std::uint64_t use_count()const { return p_impl ? p_impl->count() : 0; }
	inline RefferenceCounter& operator=(const RefferenceCounter& arg_r) {
		impl* tmp = arg_r.p_impl;
		if (tmp != p_impl) {
			if (tmp) tmp->inc();
			release();
			p_impl = tmp;
		}
		return *this;
	}
	inline RefferenceCounter CreateSameType(void* arg_p_src)const {
		return RefferenceCounter(p_impl->CreateSameTypeStorage(arg_p_src));
	}
	inline bool ShowGUI(const std::string& arg_label) {
		return p_impl->p_typeStorage->ShowGUI(arg_label);
	}
	inline void _Increase() {
		p_impl->inc();
	}
	inline void _Decrease() {
		p_impl->inc();
	}
	inline void GetRestoreObject(Value_ptr<IValuePtrRestoreObject>& arg_ref_vlp) const;
private:
	struct impl {
		template<typename S, typename D> explicit impl(S* arg_p, D)
			:use(1), p_typeStorage(D::template type<S>::name::get(arg_p)) {}
		template<typename S, typename D> explicit impl(S* arg_p, D* arg_p_typeStorage)
			:use(1), p_typeStorage(arg_p_typeStorage) {}
		impl(TypeStorage::ITypeStorage* arg_p_typeStorage):use(1),p_typeStorage(arg_p_typeStorage){}
		impl(const impl& arg_c) :use(arg_c.use), p_typeStorage(arg_c.p_typeStorage) {}
		inline std::uint64_t inc() {
			return	_InterlockedIncrement64(reinterpret_cast<volatile std::int64_t*>(&use));
		}
		inline std::uint64_t dec() {
			return	_InterlockedDecrement64(reinterpret_cast<volatile std::int64_t*>(&use));
		}

		inline std::uint64_t count()const { return use; }
		~impl() { p_typeStorage->Destroy(); }
		impl* CreateSameTypeStorage(void* arg_p_src)const {
			return new impl(p_typeStorage->CreateSameTypeStorage(arg_p_src));
		}
		std::uint64_t use;
		TypeStorage::ITypeStorage* p_typeStorage;
	};
	impl* p_impl;
};

class RefferenceCounter_weak {
public:
	constexpr RefferenceCounter_weak(const RefferenceCounter& arg_refferenceCounter) noexcept :p_impl(arg_refferenceCounter.p_impl) {}
	constexpr RefferenceCounter_weak()noexcept:p_impl(nullptr){}
	inline const RefferenceCounter_weak& operator=(const RefferenceCounter& arg_refferenceCounter) {
		p_impl = arg_refferenceCounter.p_impl;
	}
	inline operator RefferenceCounter()const {
		return RefferenceCounter(p_impl);
	}
private:
	RefferenceCounter::impl* p_impl;
};
}
}

template<typename T, typename RefferenceObject = SmartPtrDetail::RefferenceCounter::RefferenceCounter_weak>
class Value_weak_ptr {
	using this_type = Value_weak_ptr<T, RefferenceObject>;
	using refCounter_type = RefferenceObject;
	using value_type = T;
	using element_type = std::remove_all_extents_t<T>;
	using pointer = T*;
	using const_pointer = const T*;
public:
	template<typename S>
	this_type& operator=(const Value_ptr<S>& arg_other) {
		p_value = arg_other.p_value;
		refferenceCounter = arg_other.refferenceCounter;
	}
private:
	pointer p_value;
	refCounter_type refferenceCounter;
};
template<typename T, typename RefferenceObject >
class Value_ptr {
public:
	using this_type = Value_ptr<T, RefferenceObject>;
	using refCounter_type = RefferenceObject;
	using value_type = T; 
	using element_type =std::remove_all_extents_t<T>;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = SmartPtrDetail::TypeStorage::type_to_ref<T>;
	using const_reference = SmartPtrDetail::TypeStorage::type_to_constRef<T>;

	inline constexpr Value_ptr()noexcept :p_value(nullptr), refferenceCounter() {}
	inline constexpr Value_ptr(std::nullptr_t)noexcept :p_value(nullptr), refferenceCounter() {}
	inline Value_ptr(const this_type& arg_s) : p_value(arg_s.p_value), refferenceCounter(arg_s.refferenceCounter) {}
	template<typename S>
	inline Value_ptr(const Value_ptr<S, refCounter_type>& arg_s) : p_value(arg_s.p_value), refferenceCounter(arg_s.refferenceCounter) {}
	template<typename S>
	inline explicit Value_ptr(const S& arg_v) :Value_ptr(new S(arg_v)){	}
	template <class U>
	Value_ptr(const Value_ptr<U>& arg_other, element_type* arg_p) noexcept {
		refferenceCounter = arg_other.refferenceCounter;
		//refferenceCounter._Increase();
		p_value = arg_p;
	}
	inline Value_ptr(T* arg_p, RefferenceObject arg_refObj) :p_value(arg_p), refferenceCounter(arg_refObj) {}
	template<typename S>
	inline explicit Value_ptr(S* arg_p) :p_value(arg_p), refferenceCounter(arg_p, SmartPtrDetail::TypeStorage::TypeStorageSpecifier()) {
		_SetWeakPtrEnableValue(arg_p);
	}
	template<typename S>
	inline explicit Value_ptr(S* arg_p, [[maybe_unused]] const SmartPtrDetail::MemberPtrNotify) :p_value(arg_p), refferenceCounter(arg_p, SmartPtrDetail::TypeStorage::MemberTypeStorageSpecifier()) {}
	template<typename S, typename D>
	inline explicit Value_ptr(S* arg_p, D arg_d) :p_value(arg_p), refferenceCounter(arg_p, arg_d) {
		_SetWeakPtrEnableValue(arg_p);
	}
	template<typename S>
	inline constexpr Value_ptr(const Value_weak_ptr<S> arg_vwp_other)noexcept {
		ConstructFromWeak(arg_vwp_other);
	}

	~Value_ptr() {}
	inline this_type CreateSameTypeValuePtr(void* arg_p_src) const{
		return this_type(arg_p_src, refferenceCounter.CreateSameType(arg_p_src));
	}

	inline void reset() { this_type().swap(*this); }
	inline void reset(const this_type& arg_s) { this_type(arg_s).swap(*this); }
	template<class S> void reset(S* arg_p) { this_type(arg_p).swap(*this); }
	template<class S, class D>
	inline void reset(S* arg_p, D arg_d) { this_type(arg_p, arg_d).swap(*this); }
	inline void swap(this_type& arg_other) {
		std::swap(p_value, arg_other.p_value);
		refferenceCounter.swap(arg_other.refferenceCounter);
	}

	inline this_type& operator=(const this_type& arg_s) {
		p_value = arg_s.p_value;
		refferenceCounter = arg_s.refferenceCounter;
		return *this;
	}
	template<class S>
	inline this_type& operator=(const Value_ptr<S, refCounter_type>& arg_s) {
		p_value = arg_s.p_value;
		refferenceCounter = arg_s.refferenceCounter;
		return *this;
	}
	inline std::string ToString() const{
		return refferenceCounter.GetTypeStorage()->ToString();
	}
	Value_ptr<T> Clone()const {
		auto typeStorage = refferenceCounter.GetTypeStorage()->Clone();
		return Value_ptr<T>(reinterpret_cast<pointer>(typeStorage->ptr()), typeStorage);
	}
	/// <summary>
	/// データの直接書き込み
	/// </summary>
	/// <param name="arg_src">書き込み元のポインタ、型が同一と信頼</param>
	inline void Write(const void* arg_src) {
		refferenceCounter.Write(arg_src);
	}
	template<typename CastType>
	inline Value_ptr<CastType> StaticCast()const {
		auto typeStorage = refferenceCounter.GetTypeStorage()->StaticCast<T, CastType>();
		return Value_ptr<CastType>(reinterpret_cast<CastType*>(typeStorage->ptr()), typeStorage);
	}
	template<typename OwnType, typename CastType>
	inline Value_ptr<CastType> StaticCast()const {
		auto typeStorage = refferenceCounter.GetTypeStorage()->StaticCast<OwnType, CastType>();
		return Value_ptr<CastType>(reinterpret_cast<CastType*>(typeStorage->ptr()), typeStorage);
	}

	inline reference operator*() { if constexpr (std::is_same_v<element_type, void>) {} else { return *get(); } }
	inline const_reference operator*()const { if constexpr (std::is_same_v<element_type, void>) {} else { return *get(); } }
	inline pointer operator->() { return get(); }
	inline const_pointer operator->()const { return get(); }

	inline pointer get()const { return p_value; }
	template<typename RetType>
	inline RetType* get() { return reinterpret_cast<RetType*>(p_value); }
	template<typename RetType>
	inline const RetType* get()const { return reinterpret_cast<RetType*>(p_value); }

	inline bool unique()const { return refferenceCounter.unique(); }
	inline std::uint64_t use_count()const { return refferenceCounter.use_count(); }

	inline operator bool()const { return !!p_value; }
	inline operator std::int32_t() const = delete;
	inline bool operator!()const { return !p_value; }
	inline bool ShowGUI(const std::string& arg_label) {
		return refferenceCounter.ShowGUI(arg_label);
	}

	inline Value_ptr<IValuePtrRestoreObject, refCounter_type> GetRestoreObject() const {
		Value_ptr<IValuePtrRestoreObject, refCounter_type> output;
		refferenceCounter.GetRestoreObject(output);
		return output;
	}

private:
	template<typename S>
	inline bool ConstructFromWeak(const Value_weak_ptr<S>& arg_vwp_other) noexcept {
		if (arg_vwp_other.p_value) {
			p_value = arg_vwp_other.p_value;
			refferenceCounter = arg_vwp_other.refferenceCounter;
			return true;
		}

		return false;
	}
	template <class U>
	inline constexpr void _SetWeakPtrEnableValue(U* arg_p_this) noexcept {
		if constexpr (std::conjunction_v<std::negation<std::is_array<T>>, std::negation<std::is_volatile<U>>, _Can_enable_value<U>>) {
			arg_p_this->weak_ptr = *this;
		}
	}
	template<typename S, typename R> friend class Value_ptr;
	template<typename S, typename R> friend class Value_weak_ptr;
	pointer p_value;
	refCounter_type refferenceCounter;
};


template<class T, class U, class R>
inline bool operator==(const Value_ptr<T, R>& arg_lhs, const Value_ptr<U, R>& arg_rhs) {
	return arg_lhs.get() == arg_rhs.get();
}
template<class T, class U, class R>
inline bool operator!=(const Value_ptr<T, R>& arg_lhs, const Value_ptr<U, R>& arg_rhs) {
	return arg_lhs.get() != arg_rhs.get();
}
template <typename T>
class enable_value_from_this {
public:

	using _Evft_type = enable_value_from_this;

	[[nodiscard]] Value_ptr<T> value_from_this() {
		return Value_ptr<T>(weak_ptr);
	}

	[[nodiscard]] Value_ptr<const T> value_from_this() const {
		return Value_ptr<const T>(weak_ptr);
	}
protected:
	constexpr enable_value_from_this() noexcept :weak_ptr(){}

	enable_value_from_this(const enable_value_from_this&) noexcept :weak_ptr(){
	}

	enable_value_from_this& operator=(const enable_value_from_this&) noexcept {
		return *this;
	}

	~enable_value_from_this() = default;

private:
	Value_weak_ptr<T> weak_ptr;
};

template<typename T, typename... Args>
inline constexpr Value_ptr<T> make_value(Args&&... args) {
	return Value_ptr<T>(new T(args...));
}
template <typename T>
inline Value_ptr<T> to_value(T* v) {
	return Value_ptr<T>(v, SmartPtrDetail::MemberNotify);
}

template<typename T>
class ValuePtrRestoreObject :public IValuePtrRestoreObject {
public:
	ValuePtrRestoreObject(const T& arg_value):data(arg_value) {
	}
	ValuePtrRestoreObject() {
	}
	void RestoreValue(ButiEngine::Value_ptr<void>& arg_ref_value)const override;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(data);
	}
private:
	T data;
};
inline void SmartPtrDetail::RefferenceCounter::RefferenceCounter::GetRestoreObject(Value_ptr<IValuePtrRestoreObject>& arg_ref_vlp)const {
	p_impl->p_typeStorage->GetRestoreObject(arg_ref_vlp);
}
template<typename T>
inline void SmartPtrDetail::TypeStorage::TypeStorage<T>::GetRestoreObject(Value_ptr<IValuePtrRestoreObject>& arg_ref_vlp)const {
	if constexpr (!std::is_base_of_v<IValuePtrRestoreObject, T>) {
		arg_ref_vlp = make_value<ValuePtrRestoreObject<T>>(*p_value);
	}
}
template<typename T>
inline void SmartPtrDetail::TypeStorage::MemberTypeStorage<T>::GetRestoreObject(Value_ptr<IValuePtrRestoreObject>& arg_ref_vlp)const {
	if constexpr (!std::is_base_of_v<IValuePtrRestoreObject, T>) {
		arg_ref_vlp = make_value<ValuePtrRestoreObject<T>>(*p_value);
	}
}
template<typename T>
void ValuePtrRestoreObject<T>::RestoreValue(ButiEngine::Value_ptr<void>& arg_ref_value)const
{
	arg_ref_value = make_value<T>(data);
}
template<typename AfterType, typename BeforeType>
static inline constexpr Value_ptr<AfterType> dynamic_value_ptr_cast(const Value_ptr<BeforeType>& arg_before) {
	const auto temp = dynamic_cast<Value_ptr<AfterType>::element_type*>(arg_before.get());
	if (temp) {
		return Value_ptr<AfterType>(arg_before, temp);
	}
	return nullptr;
}
template<typename AfterType, typename BeforeType>
static inline constexpr Value_ptr<AfterType> dynamic_value_ptr_cast(Value_ptr<BeforeType>&& arg_before) {
	const auto temp = dynamic_cast<Value_ptr<AfterType>::element_type*>(arg_before.get());
	if (temp) {
		return Value_ptr<AfterType>(std::move(arg_before), temp);
	}
	return nullptr;
}

template<typename AfterType, typename BeforeType>
static inline constexpr Value_ptr<AfterType> static_value_ptr_cast(const Value_ptr<BeforeType>& arg_before) {
	const auto temp = static_cast<Value_ptr<AfterType>::element_type*>(arg_before.get());
	return Value_ptr<AfterType>(arg_before, temp);
}
template<typename AfterType, typename BeforeType>
static inline constexpr Value_ptr<AfterType> static_value_ptr_cast(Value_ptr<BeforeType>&& arg_before) {
	const auto temp = static_cast<Value_ptr<AfterType>::element_type*>(arg_before.get());
	return Value_ptr<AfterType>(std::move( arg_before), temp);
}
template <typename T>
static inline Value_ptr<T> CreateSameTypeValuePtr(Value_ptr<T> arg_baseValuePtr, void* arg_p_src) {
	return arg_baseValuePtr->CreateSameTypeValuePtr(arg_p_src);
}
}

#endif // !BUTIPTR_H
