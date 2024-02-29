namespace msrcgui.Models
{

    public struct Config
    {
        public UInt16 Version { get; set; }

        public UInt16 EscProtocol { get; set; }

        public float AlphaCurrent { get; set; }

        public byte PairOfPoles { get; set; }

        public byte Debug {  get; set; }

        public UInt32 Spare1 { get; set; }
        public UInt32 Spare2 { get; set; }
        public UInt32 Spare3 { get; set; }
        public UInt32 Spare4 { get; set; }
        public UInt32 Spare5 { get; set; }

    }

    public enum EscProtocol
    {
        ESC_None,
        ESC_HW5
    }
}
