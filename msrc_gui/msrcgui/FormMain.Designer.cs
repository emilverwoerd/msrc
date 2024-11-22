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
            cbPorts.Location = new Point(14, 16);
            cbPorts.Margin = new Padding(3, 4, 3, 4);
            cbPorts.Name = "cbPorts";
            cbPorts.Size = new Size(375, 28);
            cbPorts.TabIndex = 0;
            cbPorts.SelectedIndexChanged += cbPorts_SelectedIndexChanged;
            // 
            // btnConnect
            // 
            btnConnect.Location = new Point(397, 16);
            btnConnect.Margin = new Padding(3, 4, 3, 4);
            btnConnect.Name = "btnConnect";
            btnConnect.Size = new Size(95, 32);
            btnConnect.TabIndex = 1;
            btnConnect.Text = "Connect";
            btnConnect.UseVisualStyleBackColor = true;
            btnConnect.Click += btnConnect_Click;
            // 
            // btnSave
            // 
            btnSave.Location = new Point(498, 17);
            btnSave.Margin = new Padding(3, 4, 3, 4);
            btnSave.Name = "btnSave";
            btnSave.Size = new Size(86, 31);
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
            gbEscConfiguration.Location = new Point(0, 79);
            gbEscConfiguration.Margin = new Padding(3, 4, 3, 4);
            gbEscConfiguration.Name = "gbEscConfiguration";
            gbEscConfiguration.Padding = new Padding(3, 4, 3, 4);
            gbEscConfiguration.Size = new Size(600, 305);
            gbEscConfiguration.TabIndex = 4;
            gbEscConfiguration.TabStop = false;
            gbEscConfiguration.Text = "ESC Information";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(30, 185);
            label3.Name = "label3";
            label3.Size = new Size(130, 20);
            label3.TabIndex = 5;
            label3.Text = "Current Correction";
            // 
            // nudAlphaCurrent
            // 
            nudAlphaCurrent.DecimalPlaces = 2;
            nudAlphaCurrent.Increment = new decimal(new int[] { 1, 0, 0, 131072 });
            nudAlphaCurrent.Location = new Point(198, 175);
            nudAlphaCurrent.Margin = new Padding(3, 4, 3, 4);
            nudAlphaCurrent.Maximum = new decimal(new int[] { 15, 0, 0, 65536 });
            nudAlphaCurrent.Minimum = new decimal(new int[] { 5, 0, 0, 65536 });
            nudAlphaCurrent.Name = "nudAlphaCurrent";
            nudAlphaCurrent.Size = new Size(259, 27);
            nudAlphaCurrent.TabIndex = 4;
            nudAlphaCurrent.Value = new decimal(new int[] { 1, 0, 0, 0 });
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(30, 123);
            label2.Name = "label2";
            label2.Size = new Size(89, 20);
            label2.TabIndex = 3;
            label2.Text = "Pair of Poles";
            // 
            // nudPairOfPoles
            // 
            nudPairOfPoles.Location = new Point(198, 112);
            nudPairOfPoles.Margin = new Padding(3, 4, 3, 4);
            nudPairOfPoles.Maximum = new decimal(new int[] { 30, 0, 0, 0 });
            nudPairOfPoles.Minimum = new decimal(new int[] { 1, 0, 0, 0 });
            nudPairOfPoles.Name = "nudPairOfPoles";
            nudPairOfPoles.Size = new Size(259, 27);
            nudPairOfPoles.TabIndex = 2;
            nudPairOfPoles.Value = new decimal(new int[] { 1, 0, 0, 0 });
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(30, 71);
            label1.Name = "label1";
            label1.Size = new Size(34, 20);
            label1.TabIndex = 1;
            label1.Text = "ESC";
            // 
            // cbEscSelection
            // 
            cbEscSelection.FormattingEnabled = true;
            cbEscSelection.Items.AddRange(new object[] { "None", "Hobbywing V5 Platinum", "Scorpion ESC UnSync" });
            cbEscSelection.Location = new Point(198, 60);
            cbEscSelection.Margin = new Padding(3, 4, 3, 4);
            cbEscSelection.Name = "cbEscSelection";
            cbEscSelection.Size = new Size(259, 28);
            cbEscSelection.TabIndex = 0;
            // 
            // sStrip
            // 
            sStrip.ImageScalingSize = new Size(20, 20);
            sStrip.Items.AddRange(new ToolStripItem[] { tsInformationLabel });
            sStrip.Location = new Point(0, 367);
            sStrip.Name = "sStrip";
            sStrip.Padding = new Padding(1, 0, 16, 0);
            sStrip.Size = new Size(598, 22);
            sStrip.TabIndex = 5;
            // 
            // tsInformationLabel
            // 
            tsInformationLabel.Name = "tsInformationLabel";
            tsInformationLabel.Size = new Size(0, 16);
            // 
            // FormMain
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(598, 389);
            Controls.Add(sStrip);
            Controls.Add(gbEscConfiguration);
            Controls.Add(btnSave);
            Controls.Add(btnConnect);
            Controls.Add(cbPorts);
            Margin = new Padding(3, 4, 3, 4);
            MaximizeBox = false;
            MaximumSize = new Size(616, 436);
            MinimumSize = new Size(616, 436);
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
