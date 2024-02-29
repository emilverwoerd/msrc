using msrcgui.Builders;
using msrcgui.Models;
using System.IO.Ports;
using System.Runtime.InteropServices;

namespace msrcgui
{
    public partial class FormMain : Form
    {
        private const int WaitTimeOut = 4000;

        private readonly byte[] request_config = [0x30, 0x31];
        private readonly byte[] write_config = [0x30, 0x30];
        private readonly byte[] received_config = [0x30, 0x32];

        private SerialPort _serialPort;
        private List<byte> _recievedBytes = new List<byte>();

        public FormMain()
        {
            InitializeComponent();
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            cbPorts.Items.Clear();
            cbPorts.Items.AddRange(SerialPort.GetPortNames());

            DisplayStatusMessage("Select a COM Port and connect to operate the Tool");
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            LoadConfiguration();
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
           SaveConfiguration();
        }

        private void LoadConfiguration()
        {
            _serialPort = GetSerialPort();
            _serialPort.Write(request_config, 0, 2);
            Thread.Sleep(WaitTimeOut);

            if (_serialPort.BytesToRead > 0 && _serialPort.BytesToRead < 256)
            {
                var bufferSize = _serialPort.BytesToRead;
                var buffer = new byte[bufferSize];
                _serialPort.Read(buffer, 0, bufferSize);

                if (buffer[0..2].SequenceEqual(received_config))
                {
                    var config = GetConfigFromBytes(buffer[2..]);
                    cbEscSelection.SelectedIndex = config.EscProtocol;
                    nudPairOfPoles.Value = config.PairOfPoles;
                    nudAlphaCurrent.Value = (decimal)config.AlphaCurrent;

                    gbEscConfiguration.Enabled = true;
                   
                    _serialPort.Close();
                    DisplayStatusMessage("Config loaded from device");

                    return;
                }
            }

            _serialPort.Close();
            DisplayStatusMessage("Unable to load information", true);
        }

        private void SaveConfiguration()
        {
            var config = new ConfigBuilder()
                .MetEscProtocol((ushort)cbEscSelection.SelectedIndex)
                .MetPairOfPoles((byte)nudPairOfPoles.Value)
                .MetAlphaCurrent((float) nudAlphaCurrent.Value)
                .Build();

            var buffer  = GetBytesFromConfig(config);

            _serialPort = GetSerialPort();
            _serialPort.Write([.. write_config, .. buffer], 0, buffer.Length + 2);

            Thread.Sleep(WaitTimeOut);
            _serialPort.Close();

            DisplayStatusMessage("Config saved to device");
        }

        private SerialPort GetSerialPort()
        {
            var portName = cbPorts.SelectedItem as string;
            var serialPort = new SerialPort(portName, 115200);
            serialPort.DtrEnable = true; //enable Data Terminal Ready
            serialPort.RtsEnable = true; //enable Request to send

            serialPort.Open();

            return serialPort;
        }

        private void DisplayStatusMessage(string message, bool displayInBox = false)
        {
            sStrip.Items[0].Text = message;

            if (displayInBox)
            {
                MessageBox.Show(message, "Information");
            }
        }

        private static byte[] GetBytesFromConfig(Config config)
        {
            int size = Marshal.SizeOf(config);
            byte[] buffer = new byte[size];
            IntPtr ptr = Marshal.AllocHGlobal(size);
            Marshal.StructureToPtr(config, ptr, true);
            Marshal.Copy(ptr, buffer, 0, size);
            Marshal.FreeHGlobal(ptr);

            return buffer;
        }

        private static Config GetConfigFromBytes(byte[] buffer)
        {
            var config = new Config();
            int size = Marshal.SizeOf<Config>();

            IntPtr ptr = Marshal.AllocHGlobal(size);
            Marshal.Copy(buffer, 0, ptr, size);

            config = (Config)Marshal.PtrToStructure(ptr, config.GetType());
            Marshal.FreeHGlobal(ptr);

            return config;
        }

        private void cbPorts_SelectedIndexChanged(object sender, EventArgs e)
        {
            gbEscConfiguration.Enabled = false;
        }
    }
}
