#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <functional>
#include <ranges>

#include <Coral/HostInstance.hpp>
#include <Coral/GC.hpp>
#include <Coral/NativeArray.hpp>
#include <Coral/Attribute.hpp>

void ExceptionCallback(std::string_view InMessage)
{
	std::cout << "Unhandled native exception: " << InMessage << std::endl;
}

char8_t SByteMarshalIcall(char8_t InValue) { return InValue * 2; }
uint8_t ByteMarshalIcall(uint8_t InValue) { return InValue * 2; }
int16_t ShortMarshalIcall(int16_t InValue) { return InValue * 2; }
uint16_t UShortMarshalIcall(uint16_t InValue) { return InValue * 2; }
int32_t IntMarshalIcall(int32_t InValue) { return InValue * 2; }
uint32_t UIntMarshalIcall(uint32_t InValue) { return InValue * 2; }
int64_t LongMarshalIcall(int64_t InValue) { return InValue * 2; }
uint64_t ULongMarshalIcall(uint64_t InValue) { return InValue * 2; }
float FloatMarshalIcall(float InValue) { return InValue * 2.0f; }
double DoubleMarshalIcall(double InValue) { return InValue * 2.0; }
bool BoolMarshalIcall(bool InValue) { return !InValue; }
int32_t* IntPtrMarshalIcall(int32_t* InValue)
{
	*InValue *= 2;
	return InValue;
}
Coral::NativeString StringMarshalIcall(Coral::NativeString InStr)
{
	return InStr;
}
void StringMarshalIcall2(Coral::NativeString InStr)
{
	std::cout << InStr.ToString() << std::endl;
}
Coral::TypeId TypeMarshalIcall(Coral::TypeId InTypeId)
{
	return InTypeId;
}

struct DummyStruct
{
	int32_t X;
	float Y;
	int32_t Z;
};
DummyStruct DummyStructMarshalIcall(DummyStruct InStruct)
{
	InStruct.X *= 2;
	InStruct.Y *= 2.0f;
	InStruct.Z *= 2;
	return InStruct;
}

DummyStruct* DummyStructPtrMarshalIcall(DummyStruct* InStruct)
{
	InStruct->X *= 2;
	InStruct->Y *= 2.0f;
	InStruct->Z *= 2;
	return InStruct;
}

Coral::NativeArray<float> FloatArrayIcall()
{
	std::vector<float> floats = { 5.0f, 10.0f, 15.0f, 50.0f };
	return Coral::NativeArray(floats);
}

void RegisterTestInternalCalls(Coral::ManagedAssembly& InAssembly)
{
	InAssembly.AddInternalCall("Testing.Managed.Tests", "SByteMarshalIcall", reinterpret_cast<void*>(&SByteMarshalIcall));
	InAssembly.AddInternalCall("Testing.Managed.Tests", "ByteMarshalIcall", reinterpret_cast<void*>(&ByteMarshalIcall));
	InAssembly.AddInternalCall("Testing.Managed.Tests", "ShortMarshalIcall", reinterpret_cast<void*>(&ShortMarshalIcall));
	InAssembly.AddInternalCall("Testing.Managed.Tests", "UShortMarshalIcall", reinterpret_cast<void*>(&UShortMarshalIcall));
	InAssembly.AddInternalCall("Testing.Managed.Tests", "IntMarshalIcall", reinterpret_cast<void*>(&IntMarshalIcall));
	InAssembly.AddInternalCall("Testing.Managed.Tests", "UIntMarshalIcall", reinterpret_cast<void*>(&UIntMarshalIcall));
	InAssembly.AddInternalCall("Testing.Managed.Tests", "LongMarshalIcall", reinterpret_cast<void*>(&LongMarshalIcall));
	InAssembly.AddInternalCall("Testing.Managed.Tests", "ULongMarshalIcall", reinterpret_cast<void*>(&ULongMarshalIcall));
	InAssembly.AddInternalCall("Testing.Managed.Tests", "FloatMarshalIcall", reinterpret_cast<void*>(&FloatMarshalIcall));
	InAssembly.AddInternalCall("Testing.Managed.Tests", "DoubleMarshalIcall", reinterpret_cast<void*>(&DoubleMarshalIcall));
	InAssembly.AddInternalCall("Testing.Managed.Tests", "BoolMarshalIcall", reinterpret_cast<void*>(&BoolMarshalIcall));
	InAssembly.AddInternalCall("Testing.Managed.Tests", "IntPtrMarshalIcall", reinterpret_cast<void*>(&IntPtrMarshalIcall));
	InAssembly.AddInternalCall("Testing.Managed.Tests", "StringMarshalIcall", reinterpret_cast<void*>(&StringMarshalIcall));
	InAssembly.AddInternalCall("Testing.Managed.Tests", "StringMarshalIcall2", reinterpret_cast<void*>(&StringMarshalIcall2));
	InAssembly.AddInternalCall("Testing.Managed.Tests", "DummyStructMarshalIcall", reinterpret_cast<void*>(&DummyStructMarshalIcall));
	InAssembly.AddInternalCall("Testing.Managed.Tests", "DummyStructPtrMarshalIcall", reinterpret_cast<void*>(&DummyStructPtrMarshalIcall));
	InAssembly.AddInternalCall("Testing.Managed.Tests", "TypeMarshalIcall", reinterpret_cast<void*>(&TypeMarshalIcall));
	InAssembly.AddInternalCall("Testing.Managed.Tests", "TypeMarshalIcall", reinterpret_cast<void*>(&TypeMarshalIcall));
	InAssembly.AddInternalCall("Testing.Managed.Tests", "FloatArrayIcall", reinterpret_cast<void*>(&FloatArrayIcall));
}

struct Test
{
	std::string Name;
	std::function<bool()> Func;
};
std::vector<Test> tests;

void RegisterTest(std::string_view InName, std::function<bool()> InFunc)
{
	tests.emplace_back(std::string(InName), std::move(InFunc));
}

void RegisterMemberMethodTests(Coral::HostInstance& InHost, Coral::ManagedObject InObject)
{
	RegisterTest("SByteTest", [InObject]() mutable{ return InObject.InvokeMethod<char8_t, char8_t>("SByteTest", 10) == 20; });
	RegisterTest("ByteTest", [InObject]() mutable{ return InObject.InvokeMethod<uint8_t, uint8_t>("ByteTest", 10) == 20; });
	RegisterTest("ShortTest", [InObject]() mutable{ return InObject.InvokeMethod<int16_t, int16_t>("ShortTest", 10) == 20; });
	RegisterTest("UShortTest", [InObject]() mutable{ return InObject.InvokeMethod<uint16_t, uint16_t>("UShortTest", 10) == 20; });
	RegisterTest("IntTest", [InObject]() mutable{ return InObject.InvokeMethod<int32_t, int32_t>("IntTest", 10) == 20; });
	RegisterTest("UIntTest", [InObject]() mutable{ return InObject.InvokeMethod<uint32_t, uint32_t>("UIntTest", 10) == 20; });
	RegisterTest("LongTest", [InObject]() mutable{ return InObject.InvokeMethod<int64_t, int64_t>("LongTest", 10) == 20; });
	RegisterTest("ULongTest", [InObject]() mutable{ return InObject.InvokeMethod<uint64_t, uint64_t>("ULongTest", 10) == 20; });
	RegisterTest("FloatTest", [InObject]() mutable{ return InObject.InvokeMethod<float, float>("FloatTest", 10.0f) - 20.0f < 0.001f; });
	RegisterTest("DoubleTest", [InObject]() mutable{ return InObject.InvokeMethod<double, double>("DoubleTest", 10.0) - 20.0 < 0.001; });
	//RegisterTest("BoolTest", [InObject]() mutable { return InObject.InvokeMethod<bool, bool>("BoolTest", false); });
	RegisterTest("IntPtrTest", [InObject]() mutable{ int32_t v = 10; return *InObject.InvokeMethod<int32_t*, int32_t*>("IntPtrTest", &v) == 50; });
	RegisterTest("StringTest", [InObject, &InHost]() mutable
	{
		auto str = InObject.InvokeMethod<Coral::NativeString, Coral::NativeString>("StringTest", Coral::NativeString::FromUTF8("Hello"));
		return Coral::NativeString::ToUTF8(str) == "Hello, World!";
	});
	
	RegisterTest("DummyStructTest", [InObject]() mutable
	{
		DummyStruct value =
		{
			.X = 10,
			.Y = 10.0f,
			.Z = 10,
		};
		auto result = InObject.InvokeMethod<DummyStruct, DummyStruct&>("DummyStructTest", value);
		return result.X == 20 && result.Y - 20.0f < 0.001f && result.Z == 20;
	});
	RegisterTest("DummyStructPtrTest", [InObject]() mutable
	{
		DummyStruct value =
		{
			.X = 10,
			.Y = 10.0f,
			.Z = 10,
		};
		auto* result = InObject.InvokeMethod<DummyStruct*, DummyStruct*>("DummyStructPtrTest", &value);
		return result->X == 20 && result->Y - 20.0f < 0.001f && result->Z == 20;
	});

	RegisterTest("OverloadTest", [InObject]() mutable
	{
		return InObject.InvokeMethod<int32_t, int32_t>("Int32 OverloadTest(Int32)", 50) == 1050;
	});

	RegisterTest("OverloadTest", [InObject]() mutable
	{
		return InObject.InvokeMethod<float, float>("OverloadTest", 5) == 15.0f;
	});
}

void RegisterFieldMarshalTests(Coral::HostInstance& InHost, Coral::ManagedObject InObject)
{
	RegisterTest("SByteFieldTest", [InObject]() mutable
	{
		auto value = InObject.GetFieldValue<char8_t>("SByteFieldTest");
		if (value != 10)
			return false;
		InObject.SetFieldValue<char8_t>("SByteFieldTest", 20);
		value = InObject.GetFieldValue<char8_t>("SByteFieldTest");
		return value == 20;
	});

	RegisterTest("ByteFieldTest", [InObject]() mutable
	{
		auto value = InObject.GetFieldValue<uint8_t>("ByteFieldTest");
		if (value != 10)
			return false;
		InObject.SetFieldValue<uint8_t>("ByteFieldTest", 20);
		value = InObject.GetFieldValue<uint8_t>("ByteFieldTest");
		return value == 20;
	});

	RegisterTest("ShortFieldTest", [InObject]() mutable
	{
		auto value = InObject.GetFieldValue<int16_t>("ShortFieldTest");
		if (value != 10)
			return false;
		InObject.SetFieldValue<int16_t>("ShortFieldTest", 20);
		value = InObject.GetFieldValue<int16_t>("ShortFieldTest");
		return value == 20;
	});

	RegisterTest("UShortFieldTest", [InObject]() mutable
	{
		auto value = InObject.GetFieldValue<uint16_t>("UShortFieldTest");
		if (value != 10)
			return false;
		InObject.SetFieldValue<uint16_t>("UShortFieldTest", 20);
		value = InObject.GetFieldValue<uint16_t>("UShortFieldTest");
		return value == 20;
	});

	RegisterTest("IntFieldTest", [InObject]() mutable
	{
		auto value = InObject.GetFieldValue<int32_t>("IntFieldTest");
		if (value != 10)
			return false;
		InObject.SetFieldValue<int32_t>("IntFieldTest", 20);
		value = InObject.GetFieldValue<int32_t>("IntFieldTest");
		return value == 20;
	});

	RegisterTest("UIntFieldTest", [InObject]() mutable
	{
		auto value = InObject.GetFieldValue<uint32_t>("UIntFieldTest");
		if (value != 10)
			return false;
		InObject.SetFieldValue<uint32_t>("UIntFieldTest", 20);
		value = InObject.GetFieldValue<uint32_t>("UIntFieldTest");
		return value == 20;
	});

	RegisterTest("LongFieldTest", [InObject]() mutable
	{
		auto value = InObject.GetFieldValue<int64_t>("LongFieldTest");
		if (value != 10)
			return false;
		InObject.SetFieldValue<int64_t>("LongFieldTest", 20);
		value = InObject.GetFieldValue<int64_t>("LongFieldTest");
		return value == 20;
	});

	RegisterTest("ULongFieldTest", [InObject]() mutable
	{
		auto value = InObject.GetFieldValue<uint64_t>("ULongFieldTest");
		if (value != 10)
			return false;
		InObject.SetFieldValue<uint64_t>("ULongFieldTest", 20);
		value = InObject.GetFieldValue<uint64_t>("ULongFieldTest");
		return value == 20;
	});

	RegisterTest("FloatFieldTest", [InObject]() mutable
	{
		auto value = InObject.GetFieldValue<float>("FloatFieldTest");
		if (value - 10.0f > 0.001f)
			return false;
		InObject.SetFieldValue<float>("FloatFieldTest", 20);
		value = InObject.GetFieldValue<float>("FloatFieldTest");
		return value - 20.0f < 0.001f;
	});

	RegisterTest("DoubleFieldTest", [InObject]() mutable
	{
		auto value = InObject.GetFieldValue<double>("DoubleFieldTest");
		if (value - 10.0 > 0.001)
			return false;
		InObject.SetFieldValue<double>("DoubleFieldTest", 20);
		value = InObject.GetFieldValue<double>("DoubleFieldTest");
		return value - 20.0 < 0.001;
	});
	
	RegisterTest("BoolFieldTest", [InObject]() mutable
	{
		auto value = InObject.GetFieldValue<Coral::Bool32>("BoolFieldTest");
		if (value != false)
			return false;
		InObject.SetFieldValue<Coral::Bool32>("BoolFieldTest", true);
		value = InObject.GetFieldValue<Coral::Bool32>("BoolFieldTest");
		return static_cast<bool>(value);
	});
	RegisterTest("StringFieldTest", [InObject]() mutable
	{
		auto value = InObject.GetFieldValue<Coral::NativeString>("StringFieldTest");
		if (value.ToString() != "Hello")
			return false;
		InObject.SetFieldValue("StringFieldTest", Coral::NativeString::FromUTF8("Hello, World!"));
		value = InObject.GetFieldValue<Coral::NativeString>("StringFieldTest");
		return value.ToString() == "Hello, World!";
	});

	///// PROPERTIES ////

	RegisterTest("SBytePropertyTest", [InObject]() mutable
	{
		auto value = InObject.GetPropertyValue<char8_t>("SBytePropertyTest");
		if (value != 10)
			return false;
		InObject.SetPropertyValue<char8_t>("SBytePropertyTest", 20);
		value = InObject.GetPropertyValue<char8_t>("SBytePropertyTest");
		return value == 20;
	});

	RegisterTest("BytePropertyTest", [InObject]() mutable
	{
		auto value = InObject.GetPropertyValue<uint8_t>("BytePropertyTest");
		if (value != 10)
			return false;
		InObject.SetPropertyValue<uint8_t>("BytePropertyTest", 20);
		value = InObject.GetPropertyValue<uint8_t>("BytePropertyTest");
		return value == 20;
	});

	RegisterTest("ShortPropertyTest", [InObject]() mutable
	{
		auto value = InObject.GetPropertyValue<int16_t>("ShortPropertyTest");
		if (value != 10)
			return false;
		InObject.SetPropertyValue<int16_t>("ShortPropertyTest", 20);
		value = InObject.GetPropertyValue<int16_t>("ShortPropertyTest");
		return value == 20;
	});

	RegisterTest("UShortPropertyTest", [InObject]() mutable
	{
		auto value = InObject.GetPropertyValue<uint16_t>("UShortPropertyTest");
		if (value != 10)
			return false;
		InObject.SetPropertyValue<uint16_t>("UShortPropertyTest", 20);
		value = InObject.GetPropertyValue<uint16_t>("UShortPropertyTest");
		return value == 20;
	});

	RegisterTest("IntPropertyTest", [InObject]() mutable
	{
		auto value = InObject.GetPropertyValue<int32_t>("IntPropertyTest");
		if (value != 10)
			return false;
		InObject.SetPropertyValue<int32_t>("IntPropertyTest", 20);
		value = InObject.GetPropertyValue<int32_t>("IntPropertyTest");
		return value == 20;
	});

	RegisterTest("UIntPropertyTest", [InObject]() mutable
	{
		auto value = InObject.GetPropertyValue<uint32_t>("UIntPropertyTest");
		if (value != 10)
			return false;
		InObject.SetPropertyValue<uint32_t>("UIntPropertyTest", 20);
		value = InObject.GetPropertyValue<uint32_t>("UIntPropertyTest");
		return value == 20;
	});

	RegisterTest("LongPropertyTest", [InObject]() mutable
	{
		auto value = InObject.GetPropertyValue<int64_t>("LongPropertyTest");
		if (value != 10)
			return false;
		InObject.SetPropertyValue<int64_t>("LongPropertyTest", 20);
		value = InObject.GetPropertyValue<int64_t>("LongPropertyTest");
		return value == 20;
	});

	RegisterTest("ULongPropertyTest", [InObject]() mutable
	{
		auto value = InObject.GetPropertyValue<uint64_t>("ULongPropertyTest");
		if (value != 10)
			return false;
		InObject.SetPropertyValue<uint64_t>("ULongPropertyTest", 20);
		value = InObject.GetPropertyValue<uint64_t>("ULongPropertyTest");
		return value == 20;
	});

	RegisterTest("FloatPropertyTest", [InObject]() mutable
	{
		auto value = InObject.GetPropertyValue<float>("FloatPropertyTest");
		if (value - 10.0f > 0.001f)
			return false;
		InObject.SetPropertyValue<float>("FloatPropertyTest", 20);
		value = InObject.GetPropertyValue<float>("FloatPropertyTest");
		return value - 20.0f < 0.001f;
	});

	RegisterTest("DoublePropertyTest", [InObject]() mutable
	{
		auto value = InObject.GetPropertyValue<double>("DoublePropertyTest");
		if (value - 10.0 > 0.001)
			return false;
		InObject.SetPropertyValue<double>("DoublePropertyTest", 20);
		value = InObject.GetPropertyValue<double>("DoublePropertyTest");
		return value - 20.0 < 0.001;
	});
	
	RegisterTest("BoolPropertyTest", [InObject]() mutable
	{
		auto value = InObject.GetPropertyValue<Coral::Bool32>("BoolPropertyTest");
		if (value != false)
			return false;
		InObject.SetPropertyValue<Coral::Bool32>("BoolPropertyTest", true);
		value = InObject.GetPropertyValue<Coral::Bool32>("BoolPropertyTest");
		return static_cast<bool>(value);
	});
	RegisterTest("StringPropertyTest", [InObject]() mutable
	{
		auto value = InObject.GetPropertyValue<Coral::NativeString>("StringPropertyTest");
		if (value.ToString() != "Hello")
			return false;
		InObject.SetPropertyValue("StringPropertyTest", Coral::NativeString::FromUTF8("Hello, World!"));
		value = InObject.GetPropertyValue<Coral::NativeString>("StringPropertyTest");
		return value.ToString() == "Hello, World!";
	});
}

void RunTests()
{
	size_t passedTests = 0;
	for (size_t i = 0; i < tests.size(); i++)
	{
		const auto& test = tests[i];
		bool result = test.Func();
		if (result)
		{
			std::cout << "[" << i + 1 << " / " << tests.size() << " (" << test.Name << "): Passed\n";
			passedTests++;
		}
		else
		{
			std::cerr << "[" << i + 1 << " / " << tests.size() << " (" << test.Name << "): Failed\n"; 
		}
	}
	std::cout << "[NativeTest]: Done. " << passedTests << " passed, " << tests.size() - passedTests  << " failed.";
}

int main(int argc, char** argv)
{
#ifdef CORAL_TESTING_DEBUG
	const char* ConfigName = "Debug";
#else
	const char* ConfigName = "Release";
#endif

	auto exeDir = std::filesystem::path(argv[0]).parent_path();
	auto coralDir = exeDir.string();
	Coral::HostSettings settings =
	{
		.CoralDirectory = coralDir,
		.ExceptionCallback = ExceptionCallback
	};
	Coral::HostInstance hostInstance;
	hostInstance.Initialize(settings);

	auto loadContext = hostInstance.CreateAssemblyLoadContext("TestContext");

	auto assemblyPath = exeDir / "Testing.Managed.dll";
	auto& assembly = loadContext.LoadAssembly(assemblyPath.string());

	RegisterTestInternalCalls(assembly);
	assembly.UploadInternalCalls();

	auto& testsType = assembly.GetType("Testing.Managed.Tests");

	Coral::ManagedObject testsInstance = testsType.CreateInstance();
	testsInstance.InvokeMethod("RunManagedTests");
	testsInstance.Destroy();

	auto& fieldTestType = assembly.GetType("Testing.Managed.FieldMarshalTest");
	std::cout << fieldTestType.IsAssignableTo(fieldTestType) << std::endl;

	auto fieldTestObject = fieldTestType.CreateInstance();

	for (auto fieldInfo : fieldTestType.GetFields())
	{
		auto& type = fieldInfo.GetType();
		auto accessibility = fieldInfo.GetAccessibility();
		std::cout << fieldInfo.GetName() << " is " << type.GetFullName() << std::endl;

		auto attributes = fieldInfo.GetAttributes();
		for (auto attrib : attributes)
		{
			auto& attribType = attrib.GetType();

			if (attribType.GetFullName() == "Testing.Managed.DummyAttribute")
				std::cout << attrib.GetFieldValue<float>("SomeValue") << std::endl;
		}
	}

	for (auto propertyInfo : fieldTestType.GetProperties())
	{
		auto& type = propertyInfo.GetType();
		std::cout << propertyInfo.GetName() << " is " << type.GetFullName() << std::endl;

		auto attributes = propertyInfo.GetAttributes();
		for (auto attrib : attributes)
		{
			auto& attribType = attrib.GetType();

			if (attribType.GetFullName() == "Testing.Managed.DummyAttribute")
				std::cout << attrib.GetFieldValue<float>("SomeValue") << std::endl;
		}
	}
	
	auto& memberMethodTestType = assembly.GetType("Testing.Managed.MemberMethodTest");

	for (auto methodInfo : memberMethodTestType.GetMethods())
	{
		auto& type = methodInfo.GetReturnType();
		auto accessibility = methodInfo.GetAccessibility();
		std::cout << methodInfo.GetName() << ", Returns: " << type.GetFullName() << std::endl;
		const auto& parameterTypes = methodInfo.GetParameterTypes();
		for (const auto& paramType : parameterTypes)
		{
			std::cout << "\t" << paramType->GetFullName() << std::endl;
		}

		auto attributes = methodInfo.GetAttributes();
		for (auto attrib : attributes)
		{
			auto& attribType = attrib.GetType();

			if (attribType.GetFullName() == "Testing.Managed.DummyAttribute")
				std::cout << attrib.GetFieldValue<float>("SomeValue") << std::endl;
		}
	}

	auto memberMethodTest = memberMethodTestType.CreateInstance();

	RegisterFieldMarshalTests(hostInstance, fieldTestObject);
	RegisterMemberMethodTests(hostInstance, memberMethodTest);
	RunTests();

	memberMethodTest.Destroy();
	fieldTestObject.Destroy();

	auto& virtualMethodTestType1 = assembly.GetType("Testing.Managed.Override1");
	auto& virtualMethodTestType2 = assembly.GetType("Testing.Managed.Override2");

	auto instance1 = virtualMethodTestType1.CreateInstance();
	auto instance2 = virtualMethodTestType2.CreateInstance();

	instance1.InvokeMethod("TestMe");
	instance2.InvokeMethod("TestMe");

	instance1.Destroy();
	instance2.Destroy();

	hostInstance.UnloadAssemblyLoadContext(loadContext);

	Coral::GC::Collect();

	std::cin.get();

	loadContext = hostInstance.CreateAssemblyLoadContext("Fucku");
	auto& newAssembly = loadContext.LoadAssembly(assemblyPath.string());

	auto ls = newAssembly.GetLoadStatus();

	RegisterTestInternalCalls(newAssembly);
	newAssembly.UploadInternalCalls();

	auto& testsType2 = newAssembly.GetType("Testing.Managed.Tests");

	Coral::ManagedObject testsInstance2 = testsType2.CreateInstance();
	testsInstance2.InvokeMethod("RunManagedTests");
	testsInstance2.Destroy();
	std::cin.get();

	return 0;
}
