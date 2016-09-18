<GameFile>
  <PropertyGroup Name="city_menu_scene" Type="Scene" ID="949dd53d-3fbb-424f-9d42-65466f8d21ab" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Scene" Tag="581" ctype="GameNodeObjectData">
        <Size X="960.0000" Y="640.0000" />
        <Children>
          <AbstractNodeData Name="panel" ActionTag="-1524675579" Tag="632" IconVisible="False" LeftMargin="-1.7432" RightMargin="1.7432" TopMargin="0.0003" BottomMargin="-0.0003" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="960.0000" Y="640.0000" />
            <Children>
              <AbstractNodeData Name="city_name_panel" ActionTag="1593445582" Tag="633" IconVisible="False" LeftMargin="145.6230" RightMargin="151.8969" TopMargin="100.8611" BottomMargin="468.2032" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="662.4801" Y="70.9357" />
                <Children>
                  <AbstractNodeData Name="city_name_lbl" ActionTag="-1770781464" Tag="634" IconVisible="False" LeftMargin="11.7858" RightMargin="382.7933" TopMargin="19.6371" BottomMargin="21.4711" IsCustomSize="True" FontSize="16" LabelText="Your city's name:" VerticalAlignmentType="VT_Center" OutlineSize="2" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="267.9010" Y="29.8275" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="11.7858" Y="36.3848" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0178" Y="0.5129" />
                    <PreSize X="0.4044" Y="0.4205" />
                    <FontResource Type="Normal" Path="pixelmix.ttf" Plist="" />
                    <OutlineColor A="255" R="26" G="26" B="26" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="city_name_textfield" ActionTag="-1399427436" Tag="635" IconVisible="False" LeftMargin="292.3124" RightMargin="11.1187" TopMargin="24.7782" BottomMargin="23.1575" TouchEnable="True" FontSize="20" IsCustomSize="True" LabelText="" PlaceHolderText="Johnny's First Base" MaxLengthEnable="True" MaxLengthText="50" ctype="TextFieldObjectData">
                    <Size X="359.0490" Y="23.0000" />
                    <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                    <Position X="651.3614" Y="34.6575" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9832" Y="0.4886" />
                    <PreSize X="0.5420" Y="0.3242" />
                    <FontResource Type="Normal" Path="pixelmix.ttf" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="145.6230" Y="468.2032" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1517" Y="0.7316" />
                <PreSize X="0.6901" Y="0.1108" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="back_btn" ActionTag="624855446" Tag="630" IconVisible="False" LeftMargin="32.9257" RightMargin="797.0743" TopMargin="502.3745" BottomMargin="37.6255" TouchEnable="True" FontSize="24" ButtonText="Back" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="130.0000" Y="100.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="97.9257" Y="87.6255" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1020" Y="0.1369" />
                <PreSize X="0.1354" Y="0.1563" />
                <FontResource Type="Normal" Path="pixelmix.ttf" Plist="" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="title_lbl" ActionTag="1132339320" Tag="631" IconVisible="False" LeftMargin="52.3923" RightMargin="52.3925" TopMargin="3.2147" BottomMargin="550.6392" IsCustomSize="True" FontSize="36" LabelText="City Menu (does nothing)" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" OutlineSize="3" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="855.2153" Y="86.1461" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="479.9999" Y="593.7123" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.9277" />
                <PreSize X="0.8908" Y="0.1346" />
                <FontResource Type="Normal" Path="pixelmix.ttf" Plist="" />
                <OutlineColor A="255" R="26" G="26" B="26" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="invest_panel" ActionTag="1062837639" Tag="654" IconVisible="False" LeftMargin="145.6230" RightMargin="151.8969" TopMargin="191.8619" BottomMargin="377.2024" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="662.4801" Y="70.9357" />
                <Children>
                  <AbstractNodeData Name="invest_lbl" ActionTag="176670848" Tag="655" IconVisible="False" LeftMargin="9.7857" RightMargin="532.7933" TopMargin="7.6372" BottomMargin="9.4710" IsCustomSize="True" FontSize="16" LabelText="Invest:" VerticalAlignmentType="VT_Center" OutlineSize="2" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="119.9010" Y="53.8275" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="9.7857" Y="36.3848" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0148" Y="0.5129" />
                    <PreSize X="0.1810" Y="0.7588" />
                    <FontResource Type="Normal" Path="pixelmix.ttf" Plist="" />
                    <OutlineColor A="255" R="26" G="26" B="26" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="invest_button" ActionTag="1405860098" Tag="657" IconVisible="False" LeftMargin="287.1984" RightMargin="8.5508" TopMargin="4.8206" BottomMargin="5.7115" TouchEnable="True" FontSize="14" ButtonText="10 coins" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="366.7309" Y="60.4035" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="470.5638" Y="35.9133" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7103" Y="0.5063" />
                    <PreSize X="0.5536" Y="0.8515" />
                    <TextColor A="255" R="255" G="255" B="255" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="invested_lbl" ActionTag="-1689831456" Tag="11" IconVisible="False" LeftMargin="106.3417" RightMargin="383.7328" TopMargin="5.7302" BottomMargin="7.1068" IsCustomSize="True" FontSize="16" LabelText="123,456QaD" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" OutlineSize="2" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="172.4056" Y="58.0987" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="192.5445" Y="36.1561" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2906" Y="0.5097" />
                    <PreSize X="0.2602" Y="0.8190" />
                    <FontResource Type="Normal" Path="pixelmix.ttf" Plist="" />
                    <OutlineColor A="255" R="26" G="26" B="26" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="145.6230" Y="377.2024" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1517" Y="0.5894" />
                <PreSize X="0.6901" Y="0.1108" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="appeasement_panel" ActionTag="-1686059654" Tag="23" IconVisible="False" LeftMargin="146.6230" RightMargin="150.8969" TopMargin="285.8619" BottomMargin="283.2024" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="662.4801" Y="70.9357" />
                <Children>
                  <AbstractNodeData Name="appeasement_lbl" ActionTag="-713331910" Tag="24" IconVisible="False" LeftMargin="11.7858" RightMargin="528.7933" TopMargin="7.6371" BottomMargin="9.4711" IsCustomSize="True" FontSize="16" LabelText="Appease gods:" VerticalAlignmentType="VT_Center" OutlineSize="2" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="121.9010" Y="53.8275" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="11.7858" Y="36.3848" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0178" Y="0.5129" />
                    <PreSize X="0.1840" Y="0.7588" />
                    <FontResource Type="Normal" Path="pixelmix.ttf" Plist="" />
                    <OutlineColor A="255" R="26" G="26" B="26" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="appeasement_button" ActionTag="246070718" Tag="25" IconVisible="False" LeftMargin="287.1984" RightMargin="8.5508" TopMargin="4.8206" BottomMargin="5.7115" TouchEnable="True" FontSize="14" ButtonText="1 Soul" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="366.7309" Y="60.4035" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="470.5638" Y="35.9133" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7103" Y="0.5063" />
                    <PreSize X="0.5536" Y="0.8515" />
                    <TextColor A="255" R="255" G="255" B="255" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="appeased_lbl" ActionTag="-83459881" Tag="26" IconVisible="False" LeftMargin="106.3417" RightMargin="383.7328" TopMargin="5.7302" BottomMargin="7.1068" IsCustomSize="True" FontSize="16" LabelText="123,456QaD" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" OutlineSize="2" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="172.4056" Y="58.0987" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="192.5445" Y="36.1561" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2906" Y="0.5097" />
                    <PreSize X="0.2602" Y="0.8190" />
                    <FontResource Type="Normal" Path="pixelmix.ttf" Plist="" />
                    <OutlineColor A="255" R="26" G="26" B="26" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="146.6230" Y="283.2024" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1527" Y="0.4425" />
                <PreSize X="0.6901" Y="0.1108" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleY="1.0000" />
            <Position X="-1.7432" Y="639.9997" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="-0.0018" Y="1.0000" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>