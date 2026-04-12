<?xml version="1.0" encoding="UTF-8"?>
<simconf>
  <project EXPORT="discard">[APPS_DIR]/mrm</project>
  <project EXPORT="discard">[APPS_DIR]/mspsim</project>
  <project EXPORT="discard">[APPS_DIR]/avrora</project>
  <project EXPORT="discard">[APPS_DIR]/serial_socket</project>
  <project EXPORT="discard">[APPS_DIR]/powertracker</project>
  <project EXPORT="discard">[APPS_DIR]/mobility</project>
  <simulation>
    <title>My simulation</title>
    <speedlimit>1.0</speedlimit>
    <randomseed>123457</randomseed>
    <motedelay_us>1000000</motedelay_us>
    <radiomedium>
      org.contikios.cooja.radiomediums.UDGM
      <transmitting_range>50.0</transmitting_range>
      <interference_range>100.0</interference_range>
      <success_ratio_tx>1.0</success_ratio_tx>
      <success_ratio_rx>1.0</success_ratio_rx>
    </radiomedium>
    <events>
      <logoutput>80000</logoutput>
    </events>
    <motetype>
      org.contikios.cooja.mspmote.Exp5438MoteType
      <identifier>exp5438#1</identifier>
      <description>Exp5438 Mote Type exp5438#1</description>
      <source EXPORT="discard">[CONTIKI_DIR]/examples/tsch/rpl-udp/border-router-server/border-router-server.c</source>
      <commands EXPORT="discard">make border-router-server.exp5438 TARGET=exp5438</commands>
      <firmware EXPORT="copy">[CONTIKI_DIR]/examples/tsch/rpl-udp/border-router-server/border-router-server.exp5438</firmware>
      <moteinterface>org.contikios.cooja.interfaces.Position</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.RimeAddress</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.IPAddress</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.Mote2MoteRelations</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.MoteAttributes</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspClock</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspMoteID</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.Msp802154Radio</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.UsciA1Serial</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.Exp5438LED</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspDebugOutput</moteinterface>
    </motetype>
    <motetype>
      org.contikios.cooja.mspmote.Exp5438MoteType
      <identifier>exp5438#2</identifier>
      <description>Exp5438 Mote Type exp5438#2</description>
      <source EXPORT="discard">[CONTIKI_DIR]/examples/tsch/rpl-udp/border-router-client/border-router-client.c</source>
      <commands EXPORT="discard">make border-router-client.exp5438 TARGET=exp5438</commands>
      <firmware EXPORT="copy">[CONTIKI_DIR]/examples/tsch/rpl-udp/border-router-client/border-router-client.exp5438</firmware>
      <moteinterface>org.contikios.cooja.interfaces.Position</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.RimeAddress</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.IPAddress</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.Mote2MoteRelations</moteinterface>
      <moteinterface>org.contikios.cooja.interfaces.MoteAttributes</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspClock</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspMoteID</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.Msp802154Radio</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.UsciA1Serial</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.Exp5438LED</moteinterface>
      <moteinterface>org.contikios.cooja.mspmote.interfaces.MspDebugOutput</moteinterface>
    </motetype>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>119.06427818756585</x>
        <y>95.41833508956798</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>1</id>
      </interface_config>
      <motetype_identifier>exp5438#1</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>0.0</x>
        <y>0.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>2</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>0.0</x>
        <y>40.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>3</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>0.0</x>
        <y>80.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>4</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>0.0</x>
        <y>120.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>5</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>0.0</x>
        <y>160.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>6</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>40.0</x>
        <y>0.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>7</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>40.0</x>
        <y>40.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>8</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>40.0</x>
        <y>80.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>9</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>40.0</x>
        <y>120.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>10</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>40.0</x>
        <y>160.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>11</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>80.0</x>
        <y>0.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>12</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>80.0</x>
        <y>40.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>13</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>80.0</x>
        <y>80.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>14</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>80.0</x>
        <y>120.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>15</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>80.0</x>
        <y>160.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>16</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>120.0</x>
        <y>0.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>17</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>120.0</x>
        <y>40.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>18</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>120.0</x>
        <y>80.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>19</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>120.0</x>
        <y>120.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>20</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>120.0</x>
        <y>160.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>21</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>160.0</x>
        <y>0.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>22</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>160.0</x>
        <y>40.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>23</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>160.0</x>
        <y>80.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>24</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>160.0</x>
        <y>120.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>25</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>160.0</x>
        <y>160.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>26</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>200.0</x>
        <y>0.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>27</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>200.0</x>
        <y>40.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>28</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>200.0</x>
        <y>80.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>29</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>200.0</x>
        <y>120.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>30</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
    <mote>
      <breakpoints />
      <interface_config>
        org.contikios.cooja.interfaces.Position
        <x>200.0</x>
        <y>160.0</y>
        <z>0.0</z>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspClock
        <deviation>1.0</deviation>
      </interface_config>
      <interface_config>
        org.contikios.cooja.mspmote.interfaces.MspMoteID
        <id>31</id>
      </interface_config>
      <motetype_identifier>exp5438#2</motetype_identifier>
    </mote>
  </simulation>
  <plugin>
    org.contikios.cooja.plugins.SimControl
    <width>280</width>
    <z>3</z>
    <height>160</height>
    <location_x>1131</location_x>
    <location_y>1</location_y>
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.Visualizer
    <plugin_config>
      <moterelations>true</moterelations>
      <skin>org.contikios.cooja.plugins.skins.IDVisualizerSkin</skin>
      <skin>org.contikios.cooja.plugins.skins.GridVisualizerSkin</skin>
      <skin>org.contikios.cooja.plugins.skins.TrafficVisualizerSkin</skin>
      <skin>org.contikios.cooja.plugins.skins.UDGMVisualizerSkin</skin>
      <viewport>4.313636363636364 0.0 0.0 4.313636363636364 53.4363636363637 53.10909090909065</viewport>
    </plugin_config>
    <width>1022</width>
    <z>0</z>
    <height>851</height>
    <location_x>1</location_x>
    <location_y>1</location_y>
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.LogListener
    <plugin_config>
      <filter />
      <formatted_time />
      <coloring />
    </plugin_config>
    <width>804</width>
    <z>-1</z>
    <height>796</height>
    <location_x>818</location_x>
    <location_y>94</location_y>
    <minimized>true</minimized>
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.TimeLine
    <plugin_config>
      <mote>0</mote>
      <mote>1</mote>
      <mote>2</mote>
      <mote>3</mote>
      <mote>4</mote>
      <mote>5</mote>
      <mote>6</mote>
      <mote>7</mote>
      <mote>8</mote>
      <mote>9</mote>
      <mote>10</mote>
      <mote>11</mote>
      <mote>12</mote>
      <mote>13</mote>
      <mote>14</mote>
      <mote>15</mote>
      <mote>16</mote>
      <mote>17</mote>
      <mote>18</mote>
      <mote>19</mote>
      <mote>20</mote>
      <mote>21</mote>
      <mote>22</mote>
      <mote>23</mote>
      <mote>24</mote>
      <mote>25</mote>
      <mote>26</mote>
      <mote>27</mote>
      <mote>28</mote>
      <mote>29</mote>
      <mote>30</mote>
      <showRadioRXTX />
      <showRadioHW />
      <showLEDs />
      <zoomfactor>500.0</zoomfactor>
    </plugin_config>
    <width>1646</width>
    <z>-1</z>
    <height>240</height>
    <location_x>0</location_x>
    <location_y>746</location_y>
    <minimized>true</minimized>
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.ScriptRunner
    <plugin_config>
      <script>/*
 * 6TiSCH ANALYZER: 6P + RPL (Ayrıştırılmış) + PDR + LATENCY (Log Eşleşmeli)
 * Server Reply: KAPALI
 * Rapor Araligi: 10 DAKIKA
 */

TIMEOUT(4000000); 
var STOP_TIME = 3600000000; /* 60 Dakika */

/* Rapor Araligi: 10 Dakika (600.000.000 us) */
var PRINT_INTERVAL = 600000000;
var next_print_time = PRINT_INTERVAL;

var nodes = {};
/* Paket Zamanlarini Saklamak Icin: sent_times[nodeId][packetCount] = timestamp */
var sent_times = {}; 

/* Global Network Stats */
var global_hello_tx = 0;
var global_hello_rx = 0;
var global_latency_sum = 0;
var global_latency_count = 0;

function getSenderIdFromIp(ipString) {
  if (!ipString) return -1;
  var parts = ipString.trim().split(":");
  var lastPart = parts[parts.length - 1];
  return parseInt(lastPart, 16); 
}

function initNode(id) {
    if (!nodes[id]) {
      nodes[id] = { 
        add: 0, list: 0, del_cnt: 0, succ: 0, fail: 0, total: 0, 
        dio_tx: 0, dio_rx: 0, dao_tx: 0, dao_rx: 0, dis_tx: 0, dis_rx: 0,
        hello_tx: 0, hello_rx: 0 
      };
    }
    if (!sent_times[id]) {
        sent_times[id] = {};
    }
}

function printStats() {
  log.log("\n========== ISTATISTIK RAPORU (" + (time/1000000.0).toFixed(2) + " sn) ==========\n");
  
  var keys = [];
  for (var k in nodes) keys.push(parseInt(k, 10));
  keys.sort(function(a, b) { return a - b; });

  /* --- TABLO 1: 6P --- */
  log.log("[TABLO 1: 6P KONTROL MESAJLARI]\n");
  log.log("Node | ADD | LIST | DEL | SUCC | FAIL | TOTAL\n");
  log.log("-----|-----|------|-----|------|------|------\n");

  var s_add=0, s_list=0, s_del=0, s_succ=0, s_fail=0, s_total=0;
  var rpl_dio_tx=0, rpl_dio_rx=0, rpl_dao_tx=0, rpl_dao_rx=0, rpl_dis_tx=0, rpl_dis_rx=0;

  for (var i = 0; i &lt; keys.length; i++) {
    var id = keys[i];
    var n = nodes[id];
    log.log("  " + id + "  |  " + n.add + "  |  " + n.list + "  |  " + n.del_cnt + "  |  " + n.succ + "  |  " + n.fail + "  |  " + n.total + "\n");
    s_add += n.add; s_list += n.list; s_del += n.del_cnt; s_succ += n.succ; s_fail += n.fail; s_total += n.total;
    
    rpl_dio_tx += n.dio_tx; rpl_dio_rx += n.dio_rx; 
    rpl_dao_tx += n.dao_tx; rpl_dao_rx += n.dao_rx; 
    rpl_dis_tx += n.dis_tx; rpl_dis_rx += n.dis_rx;
  }
  log.log("-----|-----|------|-----|------|------|------\n");
  log.log("TOTAL|  " + s_add + "  |  " + s_list + "  |  " + s_del + "  |  " + s_succ + "  |  " + s_fail + "  |  " + s_total + "\n\n");

  /* --- TABLO 2: RPL (SADECE Tx) --- */
  log.log("[TABLO 2: RPL KONTROL MESAJLARI (Tx: Gonderilen)]\n");
  log.log("Node | DIO_Tx| DIO_Rx| DAO_Tx| DAO_Rx| DIS_Tx| DIS_Rx\n"); 
  log.log("-----|-------|-------|-------|-------|-------|-------\n");

  for (var i = 0; i &lt; keys.length; i++) {
    var id = keys[i];
    var n = nodes[id];
    log.log("  " + id + "  |   " + n.dio_tx + "   |   " + n.dio_rx + "   |   " + n.dao_tx + "   |   " + n.dao_rx + "   |   " + n.dis_tx + "   |   " + n.dis_rx + "\n");
  }
  log.log("-----|-------|-------|-------|-------|-------|-------\n");

  /* --- HESAPLAMALAR --- */
  var dao_pdr = 0;
  if (rpl_dao_tx &gt; 0) dao_pdr = (rpl_dao_rx * 100.0) / rpl_dao_tx;

  var hello_pdr = 0;
  var avg_latency = 0;
  if (global_hello_tx &gt; 0) hello_pdr = (global_hello_rx * 100.0) / global_hello_tx;
  if (global_latency_count &gt; 0) avg_latency = global_latency_sum / global_latency_count;

  log.log("\n##################################################\n");
  log.log("# AG PERFORMANS OZETI (NETWORK SUMMARY)          #\n");
  log.log("##################################################\n");
  log.log("# [DATA DUZLEMI - HELLO (Unicast)]               #\n");
  log.log("# Gonderilen (Tx)      : " + global_hello_tx + "\n");
  log.log("# Alinan     (Rx)      : " + global_hello_rx + "\n");
  log.log("# DATA PDR             : % " + hello_pdr.toFixed(2) + "\n");
  log.log("# ORTALAMA GECIKME     : " + avg_latency.toFixed(2) + " ms\n");
  log.log("# ---------------------------------------------- #\n");
  log.log("# [KONTROL DUZLEMI - RPL]                        #\n");
  log.log("# RPL DAO PDR (Unicast): % " + dao_pdr.toFixed(2) + " (Tx:" + rpl_dao_tx + " / Rx:" + rpl_dao_rx + ")\n");
  log.log("# ---------------------------------------------- #\n");
  log.log("# [KONTROL YAYINI - Multicast]                   #\n");
  log.log("# DIO Tx / Rx          : " + rpl_dio_tx + " / " + rpl_dio_rx + "\n");
  log.log("# DIS Tx / Rx          : " + rpl_dis_tx + " / " + rpl_dis_rx + "\n");
  log.log("##################################################\n");
  log.log("==============================================================\n");
}

while (true) {
  /* 1. BITIS */
  if (time &gt;= STOP_TIME) {
      log.log("\n&gt;&gt;&gt; SIMULASYON BITTI (60 DK) &lt;&lt;&lt;\n");
      printStats();
      log.testOK();
      break; 
  }

  /* 2. ARA RAPOR */
  if (time &gt;= next_print_time) {
      log.log("\n&gt;&gt;&gt; ARA RAPOR (" + (next_print_time/60000000) + ". Dakika) &lt;&lt;&lt;\n");
      printStats();
      next_print_time += PRINT_INTERVAL;
  }

  /* 3. LOG ANALIZI */
  if (msg) {
    initNode(id);
    var s = nodes[id];

    /* --- 6P --- */
    if (msg.indexOf("Send ADD") &gt;= 0 || msg.indexOf("Add request reached") &gt;= 0) { s.add++; s.total++; }
    else if (msg.indexOf("Send LIST") &gt;= 0 || msg.indexOf("List request reached") &gt;= 0) { s.list++; s.total++; }
    else if (msg.indexOf("Send DELETE") &gt;= 0 || msg.indexOf("Delete request reached") &gt;= 0) { s.del_cnt++; s.total++; }
    else if (msg.indexOf("Send SUCCESS") &gt;= 0 || msg.indexOf("cmd ADD rc SUCCESS") &gt;= 0) { s.succ++; s.total++; }
    else if (msg.indexOf("FAILED") &gt;= 0 || msg.indexOf("RSC: FAILED") &gt;= 0) { s.fail++; s.total++; }
  
    /* --- RPL --- */
    if (msg.indexOf("RPL") &gt;= 0) {
        if (msg.indexOf("Received a DIO") &gt;= 0) { s.dio_rx++; }
        if (msg.indexOf("Sending a DIO") &gt;= 0 || msg.indexOf("Sending a multicast-DIO") &gt;= 0) { s.dio_tx++; }
        if (msg.indexOf("Received a DAO") &gt;= 0) { s.dao_rx++; }
        if (msg.indexOf("Sending a DAO") &gt;= 0) { s.dao_tx++; }
        if (msg.indexOf("Received a DIS") &gt;= 0) { s.dis_rx++; }
        if (msg.indexOf("Sending a DIS") &gt;= 0) { s.dis_tx++; }
    }
    
    /* --- PDR (HELLO Tx) &amp; LATENCY START --- */
    // Log: "Sending HELLO &lt;count&gt; to ..."
    if (msg.indexOf("Sending HELLO") &gt;= 0) {
        try {
            var parts = msg.split(" ");
            var count = -1;
            // "Sending", "HELLO", "5", "to" --&gt; parts[2]
            for (var i=0; i&lt;parts.length; i++) {
                if (parts[i] === "HELLO" &amp;&amp; i+1 &lt; parts.length) {
                    count = parseInt(parts[i+1]);
                    break;
                }
            }
            
            if (count &gt;= 0) {
                s.hello_tx++;
                global_hello_tx++;
                // Zamani kaydet (Latency Start)
                sent_times[id][count] = time;
            }
        } catch(e) {}
    }

    /* --- PDR (HELLO Rx) &amp; LATENCY END --- */
    if (msg.indexOf("Received HELLO") &gt;= 0 &amp;&amp; msg.indexOf("from") &gt;= 0) {
        try {
            var parts = msg.split(" ");
            var count = -1;
            // "Received", "HELLO", "5", "from", ...
            for (var i=0; i&lt;parts.length; i++) {
                if (parts[i] === "HELLO" &amp;&amp; i+1 &lt; parts.length) {
                    count = parseInt(parts[i+1]);
                    break;
                }
            }
            
            // Kaynak IP bul
            var splitFrom = msg.split("from");
            if (splitFrom.length &gt; 1 &amp;&amp; count &gt;= 0) {
                var senderIp = splitFrom[1].trim().split(" ")[0];
                var senderId = getSenderIdFromIp(senderIp);
                
                if (senderId &gt; 0) {
                    initNode(senderId);
                    nodes[senderId].hello_rx++;
                    global_hello_rx++;

                    // Latency Hesabi (Match Check)
                    if (sent_times[senderId] &amp;&amp; sent_times[senderId][count]) {
                        var startTime = sent_times[senderId][count];
                        var latUs = time - startTime; // microsecond
                        var latMs = latUs / 1000.0;
                        
                        global_latency_sum += latMs;
                        global_latency_count++;
                        
                        delete sent_times[senderId][count];
                    }
                }
            }
        } catch(e) { }
    }
  }

  YIELD();
}</script>
      <active>true</active>
    </plugin_config>
    <width>750</width>
    <z>1</z>
    <height>799</height>
    <location_x>741</location_x>
    <location_y>188</location_y>
  </plugin>
  <plugin>
    Mobility
    <plugin_config>
      <positions EXPORT="copy">[APPS_DIR]/mobility/positions.dat</positions>
    </plugin_config>
    <width>500</width>
    <z>2</z>
    <height>200</height>
    <location_x>1161</location_x>
    <location_y>31</location_y>
  </plugin>
</simconf>

