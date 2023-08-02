﻿namespace Testing.Managed;

public class FieldMarshalTest
{

	public sbyte SByteFieldTest = 10;
	public byte ByteFieldTest = 10;
	public short ShortFieldTest = 10;
	public ushort UShortFieldTest = 10;
	public int IntFieldTest = 10;
	public uint UIntFieldTest = 10;
	public long LongFieldTest = 10;
	public ulong ULongFieldTest = 10;
	public float FloatFieldTest = 10.0f;
	public double DoubleFieldTest = 10.0;
	public bool BoolFieldTest = false;
	public string StringFieldTest = "Hello";

	public sbyte SBytePropertyTest { get; set; } = 10;
	public byte BytePropertyTest { get; set; } = 10;
	public short ShortPropertyTest { get; set; } = 10;
	public ushort UShortPropertyTest { get; set; } = 10;
	public int IntPropertyTest { get; set; } = 10;
	public uint UIntPropertyTest { get; set; } = 10;
	public long LongPropertyTest { get; set; } = 10;
	public ulong ULongPropertyTest { get; set; } = 10;
	public float FloatPropertyTest { get; set; } = 10.0f;
	public double DoublePropertyTest { get; set; } = 10.0;
	public bool BoolPropertyTest { get; set; } = false;
	public string StringPropertyTest { get; set; } = "Hello";

	private uint Test() => 10;
	
}