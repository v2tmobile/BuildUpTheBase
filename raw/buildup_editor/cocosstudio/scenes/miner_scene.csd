<GameFile>
  <PropertyGroup Name="miner_scene" Type="Scene" ID="e4edb5e7-7961-40a7-96ca-802dfe1339b7" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="miner_scene" Tag="223" ctype="GameNodeObjectData">
        <Size X="960.0000" Y="640.0000" />
        <Children>
          <AbstractNodeData Name="tilemap_nav" ActionTag="-256894720" Tag="224" IconVisible="True" LeftMargin="173.1085" RightMargin="786.8915" TopMargin="136.8814" BottomMargin="503.1186" StretchWidthEnable="False" StretchHeightEnable="False" InnerActionSpeed="1.0000" CustomSizeEnabled="False" ctype="ProjectNodeObjectData">
            <Size X="0.0000" Y="0.0000" />
            <AnchorPoint />
            <Position X="173.1085" Y="503.1186" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.1803" Y="0.7861" />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="Normal" Path="buttons/tilemap_nav.csd" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="back_btn" ActionTag="1232804101" Tag="233" IconVisible="False" LeftMargin="35.0000" RightMargin="795.0000" TopMargin="517.0000" BottomMargin="23.0000" TouchEnable="True" FontSize="24" ButtonText="Back" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="130.0000" Y="100.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="100.0000" Y="73.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.1042" Y="0.1141" />
            <PreSize X="0.1354" Y="0.1563" />
            <FontResource Type="Normal" Path="pixelmix.ttf" Plist="" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="info_panel" ActionTag="-621482901" Tag="20" IconVisible="False" LeftMargin="760.0000" BottomMargin="440.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="200.0000" Y="200.0000" />
            <Children>
              <AbstractNodeData Name="mine_rails_lbl" ActionTag="-1424291244" Tag="113" IconVisible="False" LeftMargin="32.4403" RightMargin="34.5597" TopMargin="14.7188" BottomMargin="165.2812" LabelText="Mine Rails" ctype="TextBMFontObjectData">
                <Size X="133.0000" Y="20.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="98.9403" Y="175.2812" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4947" Y="0.8764" />
                <PreSize X="0.6650" Y="0.1000" />
                <LabelBMFontFile_CNB Type="Normal" Path="pixelmix_20x2.fnt" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="mine_rails_count" ActionTag="1431279639" Tag="114" IconVisible="False" LeftMargin="28.4400" RightMargin="28.5600" TopMargin="43.7194" BottomMargin="136.2806" LabelText="XXX,XXXQaD" ctype="TextBMFontObjectData">
                <Size X="143.0000" Y="20.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="99.9400" Y="146.2806" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4997" Y="0.7314" />
                <PreSize X="0.7150" Y="0.1000" />
                <LabelBMFontFile_CNB Type="Normal" Path="pixelmix_20x2.fnt" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="mine_depth_lbl" ActionTag="5292265" Tag="141" IconVisible="False" LeftMargin="61.4403" RightMargin="63.5597" TopMargin="130.7186" BottomMargin="49.2814" LabelText="Depth" ctype="TextBMFontObjectData">
                <Size X="75.0000" Y="20.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="98.9403" Y="59.2814" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4947" Y="0.2964" />
                <PreSize X="0.3750" Y="0.1000" />
                <LabelBMFontFile_CNB Type="Normal" Path="pixelmix_20x2.fnt" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="mine_depth_count" ActionTag="1956838979" Tag="142" IconVisible="False" LeftMargin="28.4402" RightMargin="28.5598" TopMargin="159.7192" BottomMargin="20.2808" LabelText="XXX,XXXQaD" ctype="TextBMFontObjectData">
                <Size X="143.0000" Y="20.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="99.9402" Y="30.2808" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4997" Y="0.1514" />
                <PreSize X="0.7150" Y="0.1000" />
                <LabelBMFontFile_CNB Type="Normal" Path="pixelmix_20x2.fnt" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="mine_cart_lbl" ActionTag="876096813" Tag="112" IconVisible="False" LeftMargin="33.9404" RightMargin="36.0596" TopMargin="72.7187" BottomMargin="87.2813" LabelText="Minecarts&#xA;" ctype="TextBMFontObjectData">
                <Size X="130.0000" Y="40.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="98.9404" Y="107.2813" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4947" Y="0.5364" />
                <PreSize X="0.6500" Y="0.2000" />
                <LabelBMFontFile_CNB Type="Normal" Path="pixelmix_20x2.fnt" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="mine_cart_count" ActionTag="208944224" Tag="113" IconVisible="False" LeftMargin="28.4403" RightMargin="28.5597" TopMargin="101.7193" BottomMargin="78.2807" LabelText="XXX,XXXQaD" ctype="TextBMFontObjectData">
                <Size X="143.0000" Y="20.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="99.9403" Y="88.2807" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4997" Y="0.4414" />
                <PreSize X="0.7150" Y="0.1000" />
                <LabelBMFontFile_CNB Type="Normal" Path="pixelmix_20x2.fnt" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position X="760.0000" Y="440.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.7917" Y="0.6875" />
            <PreSize X="0.2083" Y="0.3125" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="dig_btn" ActionTag="1624672489" Tag="19" IconVisible="False" LeftMargin="798.9999" RightMargin="31.0001" TopMargin="515.0000" BottomMargin="25.0000" TouchEnable="True" FontSize="24" ButtonText="Deeper" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="130.0000" Y="100.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="863.9999" Y="75.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.9000" Y="0.1172" />
            <PreSize X="0.1354" Y="0.1563" />
            <FontResource Type="Normal" Path="pixelmix.ttf" Plist="" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="explode_btn" ActionTag="1201039480" Tag="74" IconVisible="False" LeftMargin="393.3486" RightMargin="436.6514" TopMargin="23.4773" BottomMargin="516.5227" TouchEnable="True" FontSize="24" ButtonText="Altar" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="130.0000" Y="100.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="458.3486" Y="566.5227" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.4774" Y="0.8852" />
            <PreSize X="0.1354" Y="0.1563" />
            <FontResource Type="Normal" Path="pixelmix.ttf" Plist="" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="chance_btn" ActionTag="-1763921240" Tag="578" IconVisible="False" LeftMargin="573.3481" RightMargin="256.6519" TopMargin="23.4774" BottomMargin="516.5226" TouchEnable="True" FontSize="24" ButtonText="Crypt" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="130.0000" Y="100.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="638.3481" Y="566.5226" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.6649" Y="0.8852" />
            <PreSize X="0.1354" Y="0.1563" />
            <FontResource Type="Normal" Path="pixelmix.ttf" Plist="" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>