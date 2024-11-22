using msrcgui.Models;

namespace msrcgui.Builders
{
    internal class ConfigBuilder
    {
        private byte _debug = 0;
        private UInt16 _version = 2;

        private ushort _escProtocol = 0;
        private byte _pairOfPoles = 1;
        private float _alphaCurrent = 1;

        public ConfigBuilder WithEscProtocol(ushort escProtocol)
        {
            _escProtocol = escProtocol;
            return this;
        }

        public ConfigBuilder WithPairOfPoles(byte pairOfPoles)
        {
            _pairOfPoles = pairOfPoles;
            return this;
        }

        public ConfigBuilder WithAlphaCurrent(float alphaCurrrent)
        {
            _alphaCurrent = alphaCurrrent;
            return this;
        }

        public Config Build() =>
         new Config()
         {
             Debug = _debug,
             Version = _version,
             EscProtocol = _escProtocol,
             PairOfPoles = _pairOfPoles,
             AlphaCurrent = _alphaCurrent,
         };
    }
}
