<GameFile>
  <PropertyGroup Name="harvest_scene" Type="Scene" ID="8ce07d85-9e14-413f-8700-3b9067bcad23" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Scene" Tag="239" ctype="GameNodeObjectData">
        <Size X="960.0000" Y="640.0000" />
        <Children>
          <AbstractNodeData Name="shop_pos" ActionTag="60628340" Tag="266" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" RightMargin="760.0000" TopMargin="29.8560" BottomMargin="510.1440" Scale9Width="46" Scale9Height="46" ctype="ImageViewObjectData">
            <Size X="200.0000" Y="100.0000" />
            <AnchorPoint />
            <Position Y="510.1440" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition Y="0.7971" />
            <PreSize X="0.2083" Y="0.1563" />
            <FileData Type="Default" Path="Default/ImageFile.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="city_pos" ActionTag="-1746362178" Tag="77" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" RightMargin="760.0000" TopMargin="191.8400" BottomMargin="348.1600" Scale9Width="46" Scale9Height="46" ctype="ImageViewObjectData">
            <Size X="200.0000" Y="100.0000" />
            <AnchorPoint />
            <Position Y="348.1600" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition Y="0.5440" />
            <PreSize X="0.2083" Y="0.1563" />
            <FileData Type="Default" Path="Default/ImageFile.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="building_info_panel" ActionTag="520425261" Tag="141" IconVisible="False" LeftMargin="748.6649" RightMargin="21.3351" TopMargin="25.4918" BottomMargin="394.5082" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" ctype="PanelObjectData">
            <Size X="190.0000" Y="220.0000" />
            <Children>
              <AbstractNodeData Name="info_title" ActionTag="1877563915" Tag="142" IconVisible="False" LeftMargin="38.7570" RightMargin="47.2430" TopMargin="13.4007" BottomMargin="183.5993" FontSize="20" LabelText="Building info" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="104.0000" Y="23.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="90.7570" Y="195.0993" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4777" Y="0.8868" />
                <PreSize X="0.5474" Y="0.1045" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="waste_count" ActionTag="1780408824" Tag="154" IconVisible="False" LeftMargin="133.7350" RightMargin="-6.7350" TopMargin="46.4579" BottomMargin="155.5421" FontSize="16" LabelText="WST: XX" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="63.0000" Y="18.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="165.2350" Y="164.5421" />
                <Scale ScaleX="0.7283" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8697" Y="0.7479" />
                <PreSize X="0.3316" Y="0.0818" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="product_count" ActionTag="-1782884436" Tag="155" IconVisible="False" LeftMargin="68.2230" RightMargin="57.7770" TopMargin="46.4579" BottomMargin="155.5421" FontSize="16" LabelText="PRO: XX" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="64.0000" Y="18.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="100.2230" Y="164.5421" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5275" Y="0.7479" />
                <PreSize X="0.3368" Y="0.0818" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="ingredient_count" ActionTag="-435758994" Tag="156" IconVisible="False" LeftMargin="5.2204" RightMargin="128.7796" TopMargin="46.4579" BottomMargin="155.5421" FontSize="16" LabelText="ING: XX" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="56.0000" Y="18.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="33.2204" Y="164.5421" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1748" Y="0.7479" />
                <PreSize X="0.2947" Y="0.0818" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="harvester_count" ActionTag="-1638508918" Tag="157" IconVisible="False" LeftMargin="18.4051" RightMargin="26.5949" TopMargin="106.1792" BottomMargin="95.8208" FontSize="16" LabelText="Robo-harvesters: XX" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="145.0000" Y="18.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="90.9051" Y="104.8208" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4784" Y="0.4765" />
                <PreSize X="0.7632" Y="0.0818" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="inventory_title" ActionTag="-189399336" Tag="158" IconVisible="False" LeftMargin="60.8926" RightMargin="69.1074" TopMargin="74.5601" BottomMargin="122.4399" FontSize="20" LabelText="Details" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="60.0000" Y="23.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="90.8926" Y="133.9399" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4784" Y="0.6088" />
                <PreSize X="0.3158" Y="0.1045" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position X="748.6649" Y="394.5082" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.7799" Y="0.6164" />
            <PreSize X="0.1979" Y="0.3438" />
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