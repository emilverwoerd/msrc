namespace msrcgui
{
    partial class FormMain
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            cbPorts = new ComboBox();
            btnConnect = new Button();
            btnSave = new Button();
            gbEscConfiguration = new GroupBox();
            label3 = new Label();
            nudAlphaCurrent = new NumericUpDown();
            label2 = new Label();
            nudPairOfPoles = new NumericUpDown();
            label1 = new Label();
            cbEscSelection = new ComboBox();
            sStrip = new StatusStrip();
            tsInformationLabel = new ToolStripStatusLabel();
            gbEscConfiguration.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)nudAlphaCurrent).BeginInit();
            ((System.ComponentModel.ISupportInitialize)nudPairOfPoles).BeginInit();
            sStrip.SuspendLayout();
            SuspendLayout();
            // 
            // cbPorts
            // 
            cbPorts.FormattingEnabled = true;
            cbPorts.Location = new Point(12, 12);
            cbPorts.Name = "cbPorts";
            cbPorts.Size = new Size(329, 23);
            cbPorts.TabIndex = 0;
            cbPorts.SelectedIndexChanged += cbPorts_SelectedIndexChanged;
            // 
            // btnConnect
            // 
            btnConnect.Location = new Point(347, 12);
            btnConnect.Name = "btnConnect";
            btnConnect.Size = new Size(83, 24);
            btnConnect.TabIndex = 1;
            btnConnect.Text = "Connect";
            btnConnect.UseVisualStyleBackColor = true;
            btnConnect.Click += btnConnect_Click;
            // 
            // btnSave
            // 
            btnSave.Location = new Point(436, 13);
            btnSave.Name = "btnSave";
            btnSave.Size = new Size(75, 23);
            btnSave.TabIndex = 3;
            btnSave.Text = "Save";
            btnSave.UseVisualStyleBackColor = true;
            btnSave.Click += btnSave_Click;
            // 
            // gbEscConfiguration
            // 
            gbEscConfiguration.Controls.Add(label3);
            gbEscConfiguration.Controls.Add(nudAlphaCurrent);
            gbEscConfiguration.Controls.Add(label2);
            gbEscConfiguration.Controls.Add(nudPairOfPoles);
            gbEscConfiguration.Controls.Add(label1);
            gbEscConfiguration.Controls.Add(cbEscSelection);
            gbEscConfiguration.Enabled = false;
            gbEscConfiguration.Location = new Point(0, 59);
            gbEscConfiguration.Name = "gbEscConfiguration";
            gbEscConfiguration.Size = new Size(525, 229);
            gbEscConfiguration.TabIndex = 4;
            gbEscConfiguration.TabStop = false;
            gbEscConfiguration.Text = "ESC Information";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(26, 139);
            label3.Name = "label3";
            label3.Size = new Size(106, 15);
            label3.TabIndex = 5;
            label3.Text = "Current Correction";
            // 
            // nudAlphaCurrent
            // 
            nudAlphaCurrent.DecimalPlaces = 2;
            nudAlphaCurrent.Increment = new decimal(new int[] { 1, 0, 0, 131072 });
            nudAlphaCurrent.Location = new Point(173, 131);
            nudAlphaCurrent.Maximum = new decimal(new int[] { 15, 0, 0, 65536 });
            nudAlphaCurrent.Minimum = new decimal(new int[] { 5, 0, 0, 65536 });
            nudAlphaCurrent.Name = "nudAlphaCurrent";
            nudAlphaCurrent.Size = new Size(227, 23);
            nudAlphaCurrent.TabIndex = 4;
            nudAlphaCurrent.Value = new decimal(new int[] { 1, 0, 0, 0 });
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(26, 92);
            label2.Name = "label2";
            label2.Size = new Size(72, 15);
            label2.TabIndex = 3;
            label2.Text = "Pair of Poles";
            // 
            // nudPairOfPoles
            // 
            nudPairOfPoles.Location = new Point(173, 84);
            nudPairOfPoles.Maximum = new decimal(new int[] { 30, 0, 0, 0 });
            nudPairOfPoles.Minimum = new decimal(new int[] { 1, 0, 0, 0 });
            nudPairOfPoles.Name = "nudPairOfPoles";
            nudPairOfPoles.Size = new Size(227, 23);
            nudPairOfPoles.TabIndex = 2;
            nudPairOfPoles.Value = new decimal(new int[] { 1, 0, 0, 0 });
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(26, 53);
            label1.Name = "label1";
            label1.Size = new Size(27, 15);
            label1.TabIndex = 1;
            label1.Text = "ESC";
            // 
            // cbEscSelection
            // 
            cbEscSelection.FormattingEnabled = true;
            cbEscSelection.Items.AddRange(new object[] { "None", "Hobbywing V5 Platinum" });
            cbEscSelection.Location = new Point(173, 45);
            cbEscSelection.Name = "cbEscSelection";
            cbEscSelection.Size = new Size(227, 23);
            cbEscSelection.TabIndex = 0;
            // 
            // sStrip
            // 
            sStrip.Items.AddRange(new ToolStripItem[] { tsInformationLabel });
            sStrip.Location = new Point(0, 278);
            sStrip.Name = "sStrip";
            sStrip.Size = new Size(525, 22);
            sStrip.TabIndex = 5;
            // 
            // tsInformationLabel
            // 
            tsInformationLabel.Name = "tsInformationLabel";
            tsInformationLabel.Size = new Size(0, 17);
            // 
            // FormMain
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(525, 300);
            Controls.Add(sStrip);
            Controls.Add(gbEscConfiguration);
            Controls.Add(btnSave);
            Controls.Add(btnConnect);
            Controls.Add(cbPorts);
            MaximizeBox = false;
            MaximumSize = new Size(541, 339);
            MinimumSize = new Size(541, 339);
            Name = "FormMain";
            Text = "MSRC Configuration";
            Load += FormMain_Load;
            gbEscConfiguration.ResumeLayout(false);
            gbEscConfiguration.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)nudAlphaCurrent).EndInit();
            ((System.ComponentModel.ISupportInitialize)nudPairOfPoles).EndInit();
            sStrip.ResumeLayout(false);
            sStrip.PerformLayout();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private ComboBox cbPorts;
        private Button btnConnect;
        private Button btnSave;
        private GroupBox gbEscConfiguration;
        private ComboBox cbEscSelection;
        private Label label1;
        private NumericUpDown nudAlphaCurrent;
        private Label label2;
        private NumericUpDown nudPairOfPoles;
        private StatusStrip sStrip;
        private Label label3;
        private ToolStripStatusLabel tsInformationLabel;
    }
}
