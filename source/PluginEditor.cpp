/*
 ==============================================================================
 
 This file was auto-generated!
 
 It contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>
#include <regex>
//==============================================================================

ElectroAudioProcessorEditor::ElectroAudioProcessorEditor (ElectroAudioProcessor& p, AudioProcessorValueTreeState& vts) :
AudioProcessorEditor (&p),
processor (p),
vts(vts),
OSCILLOSCOPE(processor.getAudioBufferQueue()),
tabs(TabbedButtonBar::Orientation::TabsAtTop),
keyboard(p.keyboardState, MidiKeyboardComponent::Orientation::horizontalKeyboard),
envsAndLFOs(TabbedButtonBar::TabsAtTop),
tuningTab(*this, processor, vts),
copedentTable(*this, processor, vts),
fxTab(*this, processor,vts),
constrain(new ComponentBoundsConstrainer()),
resizer(new ResizableCornerComponent (this, constrain.get())),
chooser("Select a .wav file to load...", {}, "*.wav")

{
    sysexOut = MidiOutput::openDevice(MidiOutput::getDefaultDevice().name);

    for (int i = 0; i< 512; i++)
    {
        knobMidiArray[i] = 0;
    }
    
    //settings.get()->setValue("lastStateFile",String());
    addMouseListener(this, true);
    white_circle_image = Drawable::createFromImageData(BinaryData::White_Circle_svg_png,
                                                       BinaryData::White_Circle_svg_pngSize);
    hamburger_menu_image = Drawable::createFromImageData(BinaryData::Hamburger_icon_svg_png,
                                                         BinaryData::Hamburger_icon_svg_pngSize);
    hamburger_menu_image->replaceColour(Colours::white,  Colours::gold.withBrightness(0.9f));
    //vts.state.addListener(this);
    LookAndFeel::setDefaultLookAndFeel(ElectroLookAndFeel::getInstance());
    tabs.getTabbedButtonBar().setLookAndFeel(&laf);
    meters.setChannelFormat(juce::AudioChannelSet::stereo());
    
    tab1.addAndMakeVisible (meters);
    sd::SoundMeter::Options meterOptions;
    
    meterOptions.faderEnabled = true;  // Enable or disable the 'fader' overlay. Use the sd::SoundMeter::MetersComponent::FadersChangeListener to get the fader value updates.
    
    meterOptions.headerEnabled         = true;           // Enable the 'header' part above the meter, displaying the channel ID.
    meterOptions.valueEnabled          = true;           // Enable the 'value' part below the level, displaying the peak level.
    meterOptions.refreshRate           = 30;  // Frequency of the meter updates (when using the internal timer).
    meterOptions.useGradient           = true;            // Use gradients to fill the meter (hard segment boundaries otherwise).
    meterOptions.showPeakHoldIndicator = false;           // Show the peak hold indicator (double click value to reset).
    meterOptions.tickMarksEnabled      = true;            // Enable tick-marks. Divider lines at certain levels on the meter and label strip.
    meterOptions.tickMarksOnTop        = true;            // Put the tick-marks above the level readout.
    meterOptions.tickMarks             = { -1.0f, -3.0f, -6.0f, -12.0f };  // Positions (in decibels) of the tick-marks.
    meterOptions.decayTime_ms          = 1000.0f;                                          // The meter will take 1000 ms to decay to 0.
    
    
    meters.setOptions (meterOptions);
    meters.setLabelStripPosition (sd::SoundMeter::LabelStripPosition::right);
    meters.addFadersListener(*this);
    
    Typeface::Ptr tp = Typeface::createSystemTypefaceFor(BinaryData::EuphemiaCAS_ttf,
                                                         BinaryData::EuphemiaCAS_ttfSize);
    euphemia = Font(tp);
    addAndMakeVisible(tooltipWindow);
    logo = Drawable::createFromImageData (BinaryData::logo_large_svg,
                                          BinaryData::logo_large_svgSize);
    addAndMakeVisible(logo.get());
    synderphonicsLabel.setText("SNYDERPHONICS", dontSendNotification);
    synderphonicsLabel.setJustificationType(Justification::topLeft);
    synderphonicsLabel.setColour(Label::textColourId, Colours::gold.withBrightness(0.9f));
    addAndMakeVisible(synderphonicsLabel);
    //ElectrobassLabel.setText("Electrobass", dontSendNotification);
    //ElectrobassLabel.setJustificationType(Justification::topLeft);
    //ElectrobassLabel.setColour(Label::textColourId, Colours::gold.withBrightness(0.9f));
    //addAndMakeVisible(ElectrobassLabel);
//    if (!JUCEApplicationBase::isStandaloneApp())
//    {
    
//    }
    setWantsKeyboardFocus(true);
    
    getTopLevelComponent()->addKeyListener(this);
    
    //==============================================================================
    // TAB1 ========================================================================
    addAndMakeVisible(tab1);
    
    currentMappingSource = nullptr;
    uniqueMacroComponent.setOutlineColour(Colours::darkgrey);
    tab1.addAndMakeVisible(uniqueMacroComponent);
    for (int i = 0; i < NUM_MACROS; ++i)
    {
        String n = i < NUM_GENERIC_MACROS ? "M" + String(i+1) :
        cUniqueMacroNames[i-NUM_GENERIC_MACROS];
        
        macroDials.add(new ElectroDial(*this, n, n, true, false));
        sliderAttachments.add(new SliderAttachment(vts, n, macroDials[i]->getSlider()));
        macroDials[i]->getSlider().addListener(this);
        if (i < NUM_GENERIC_MACROS) tab1.addAndMakeVisible(macroDials[i]);
        else uniqueMacroComponent.addAndMakeVisible(macroDials[i]);
    }
    
        
    // MIDI Key source
    midiKeyComponent.setOutlineColour(Colours::darkgrey);
    tab1.addAndMakeVisible(midiKeyComponent);
    
    midiKeySource = std::make_unique<MappingSource>(*this, *processor.midiKeySource,
                                                    "MIDI Key In");
    midiKeyComponent.addAndMakeVisible(midiKeySource.get());
    
    midiKeyRangeSlider.setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);
    midiKeyRangeSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 10, 10);
    midiKeyRangeSlider.setRange(0, 127, 1);
    midiKeyRangeSlider.addListener(this);
    midiKeyComponent.addAndMakeVisible(midiKeyRangeSlider);
    midiKeyRangeSlider.setName("Midi Key range");
    midiKeyMinLabel.setEditable(true);
    midiKeyMinLabel.setJustificationType(Justification::centred);
    midiKeyMinLabel.setColour(Label::backgroundColourId, Colours::darkgrey.withBrightness(0.2f));
    midiKeyMinLabel.addListener(this);
    midiKeyComponent.addAndMakeVisible(midiKeyMinLabel);
    
    midiKeyMaxLabel.setEditable(true);
    midiKeyMaxLabel.setJustificationType(Justification::centred);
    midiKeyMaxLabel.setColour(Label::backgroundColourId, Colours::darkgrey.withBrightness(0.2f));
    midiKeyMaxLabel.addListener(this);
    midiKeyComponent.addAndMakeVisible(midiKeyMaxLabel);

    // Velocity source
    velocityComponent.setOutlineColour(Colours::darkgrey);
    tab1.addAndMakeVisible(velocityComponent);

    velocitySource = std::make_unique<MappingSource>(*this, *processor.velocitySource,
                                                     "Velocity In");
    velocityComponent.addAndMakeVisible(velocitySource.get());

    // Random source
    randomComponent.setOutlineColour(Colours::darkgrey);
    tab1.addAndMakeVisible(randomComponent);
    
    randomSource = std::make_unique<MappingSource>(*this, *processor.randomSource,
                                                   "Random on Attack");
    randomComponent.addAndMakeVisible(randomSource.get());
    
    randomValueLabel.setEditable(false);
    randomValueLabel.setJustificationType(Justification::centred);
    randomValueLabel.setColour(Label::backgroundColourId, Colours::darkgrey.withBrightness(0.2f));
    randomValueLabel.addListener(this);
    randomComponent.addAndMakeVisible(randomValueLabel);
    
    // Transpose, Poly
   
    
    numVoicesLabel.setText("Voices", dontSendNotification);
    numVoicesLabel.setJustificationType(Justification::centred);
    tab1.addAndMakeVisible(numVoicesLabel);
    //numVoicesSlider.setMouseDragSensitivity(40);
    numVoicesSlider.setRange(1., (float)MAX_NUM_VOICES, 1.); //EBSPECIFIC
    numVoicesSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    numVoicesSlider.setSliderSnapsToMousePosition(false);
    numVoicesSlider.setMouseDragSensitivity(2400);
    numVoicesSlider.setSliderSnapsToMousePosition(false);
    numVoicesSlider.setTextValueSuffix(""); //EBSPECIFIC
    numVoicesSlider.setColour(Slider::backgroundColourId, Colours::darkgrey.withBrightness(0.2f));
    numVoicesSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);
    numVoicesSlider.setColour(Slider::textBoxTextColourId, Colours::gold.withBrightness(0.95f));
    numVoicesSlider.addListener(this); //EBSPECIFIC removes ability to change
    tab1.addAndMakeVisible(numVoicesSlider);
    
    transposeLabel.setText("Transpose", dontSendNotification);
    transposeLabel.setJustificationType(Justification::centred);
    tab1.addAndMakeVisible(transposeLabel);
    transposeSlider.setName("Transpose");
    transposeSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    transposeSlider.setSliderSnapsToMousePosition(false);
    transposeSlider.setMouseDragSensitivity(400);
    transposeSlider.addListener(this);
    transposeSlider.setColour(Slider::backgroundColourId, Colours::darkgrey.withBrightness(0.2f));
    transposeSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);
    transposeSlider.setColour(Slider::textBoxTextColourId, Colours::gold.withBrightness(0.95f));
    sliderAttachments.add(new SliderAttachment(vts, "Transpose", transposeSlider));
    tab1.addAndMakeVisible(transposeSlider);
    masterSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    sliderAttachments.add(new SliderAttachment(vts, "Master", masterSlider));
    masterSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 10, 10);
    masterSlider.setName("Master");
    masterSlider.addListener(this);
    tab1.addAndMakeVisible(masterSlider);
    for (int i = 0; i < NUM_CHANNELS; ++i)
    {
      
        if (i == 0)
        {
            
            stringActivityButtons.add(new TextButton("PBin"));
            stringActivityButtons[i]->setConnectedEdges(Button::ConnectedOnLeft &
                                                        Button::ConnectedOnRight);
            stringActivityButtons[i]->setInterceptsMouseClicks(false, false);
            stringActivityButtons[i]->setClickingTogglesState(false);//(true);
            stringActivityButtons[i]->addListener(this);
            tab1.addAndMakeVisible(stringActivityButtons[i]);
        } else
        {
            DrawableButton *dbutton = new DrawableButton("", DrawableButton::ButtonStyle::ImageFitted);
            dbutton->setImages(white_circle_image.get());
            stringActivityButtons.add(dbutton);
            stringActivityButtons[i]->setConnectedEdges(Button::ConnectedOnLeft &
                                                        Button::ConnectedOnRight);
            stringActivityButtons[i]->setInterceptsMouseClicks(false, false);
            stringActivityButtons[i]->setClickingTogglesState(false);//(true);
            stringActivityButtons[i]->addListener(this);
            tab1.addAndMakeVisible(stringActivityButtons[i]);
        }
        
       if (i == 0) { //EBSPECIFIC
            pitchBendSliders.add(new Slider());
            pitchBendSliders[i]->setSliderStyle(Slider::SliderStyle::LinearBar);
            pitchBendSliders[i]->setInterceptsMouseClicks(false, false);
            pitchBendSliders[i]->setColour(Slider::trackColourId, Colours::lightgrey);
            pitchBendSliders[i]->setColour(Slider::backgroundColourId, Colours::black);
            pitchBendSliders[i]->setColour(Slider::textBoxOutlineColourId, Colours::grey);
            //pitchBendSliders[i]->setTextValueSuffix("m2");
            pitchBendSliders[i]->addListener(this);
            tab1.addAndMakeVisible(pitchBendSliders[i]);
            
            sliderAttachments.add(new SliderAttachment(vts, "PitchBend" + String(i),
                                                       *pitchBendSliders[i]));
        }
        
    
    }
    
    rangeSlider.setSliderStyle(Slider::SliderStyle::LinearBar);
    rangeSlider.setInterceptsMouseClicks(true, true);
    rangeSlider.setColour(Slider::trackColourId, Colours::lightgrey);
    rangeSlider.setColour(Slider::backgroundColourId, Colours::black);
    rangeSlider.setColour(Slider::textBoxOutlineColourId, Colours::grey);
    //pitchBendSliders[i]->setTextValueSuffix("m2");
    rangeSlider.addListener(this);
    tab1.addAndMakeVisible(rangeSlider);
    
    sliderAttachments.add(new SliderAttachment(vts, "PitchBendRange",
                                               rangeSlider));
    rangeLabel.setText("RANGE", dontSendNotification);
    rangeLabel.setJustificationType(Justification::centredRight);
    tab1.addAndMakeVisible(rangeLabel);
   
    
    
    mpeToggle.setButtonText("MPE");
    mpeToggle.addListener(this);
    tab1.addAndMakeVisible(mpeToggle);
    
    muteToggle.setButtonText("Mute");
    muteToggle.addListener(this);
    pedalControlsMasterToggle.setButtonText("Pedal Volume Control");
    pedalControlsMasterToggle.addListener(this);
      
    buttonAttachments.add(new ButtonAttachment(vts, "PedalControlsMaster", pedalControlsMasterToggle));
    pedalControlsMasterToggle.setName("PedalControlsMaster");
    tab1.addAndMakeVisible(muteToggle);
    tab1.addAndMakeVisible(pedalControlsMasterToggle);
    keyboard.setAvailableRange(21, 108);
    keyboard.setOctaveForMiddleC(4);
    //    tab1.addAndMakeVisible(&keyboard);
    
    tab1.addAndMakeVisible(&envsAndLFOs);
    
    for (int i = 0; i < NUM_OSCS; ++i)
    {
        oscModules.add(new OscModule(*this, vts, *processor.oscs[i]));
        tab1.addAndMakeVisible(oscModules[i]);
    }
    
    noiseModule = std::make_unique<NoiseModule>(*this, vts, *processor.noise);
    tab1.addAndMakeVisible(noiseModule.get());
    
    for (int i = 0; i < NUM_FILT; ++i)
    {
        filterModules.add(new FilterModule(*this, vts, *processor.filt[i]));
        tab1.addAndMakeVisible(filterModules[i]);
    }
    
    
    seriesParallelComponent.setOutlineColour(Colours::darkgrey);
    tab1.addAndMakeVisible(seriesParallelComponent);
    
    seriesParallelSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    seriesParallelSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 10, 10);
    seriesParallelSlider.setName("Filter Series-Parallel Mix");
    seriesParallelComponent.addAndMakeVisible(seriesParallelSlider);
    sliderAttachments.add(new SliderAttachment(vts, "Filter Series-Parallel Mix",
                                               seriesParallelSlider));
    seriesParallelSlider.addListener(this);
    seriesLabel.setText("Ser.", dontSendNotification);
    seriesParallelComponent.addAndMakeVisible(seriesLabel);
    
    parallelLabel.setText("Par.", dontSendNotification);
    parallelLabel.setJustificationType(Justification::centredRight);
    seriesParallelComponent.addAndMakeVisible(parallelLabel);
    
    outputModule = std::make_unique<OutputModule>(*this, vts, *processor.output);
    tab1.addAndMakeVisible(outputModule.get());

    for (int i = 0; i < NUM_ENVS; ++i)
    {
        String paramName = "Envelope" + String(i+1);
        String displayName = "Env" + String(i+1);
        envsAndLFOs.addTab(" ", Colours::black, new EnvModule(*this, vts, *processor.envs[i]), true);
        envsAndLFOs.setColour(TabbedComponent::outlineColourId, Colours::darkgrey);
        
        TabbedButtonBar& bar = envsAndLFOs.getTabbedButtonBar();
        bar.getTabButton(i)
        ->setExtraComponent(new MappingSource(*this, *processor.getMappingSource(paramName), displayName),
                            TabBarButton::ExtraComponentPlacement::afterText);
        bar.setColour(TabbedButtonBar::tabOutlineColourId, Colours::darkgrey);
        bar.setColour(TabbedButtonBar::frontOutlineColourId, Colours::darkgrey);
        for (int i = 0; i < envsAndLFOs.getTabbedButtonBar().getNumTabs(); ++i)
        {
            bar.getTabButton(i)->setAlpha(i == 0 ? 1.0f : 0.5f);
        }
    }
    for (int i = 0; i < NUM_LFOS; ++i)
    {
        String paramName = "LFO" + String(i+1);
        String displayName = paramName;
        envsAndLFOs.addTab(" ", Colours::black,
                           new LFOModule(*this, vts, *processor.lfos[i]), true);
        envsAndLFOs.setColour(TabbedComponent::outlineColourId, Colours::darkgrey);
        
        TabbedButtonBar& bar = envsAndLFOs.getTabbedButtonBar();
        bar.getTabButton(i + NUM_ENVS)
        ->setExtraComponent(new MappingSource(*this, *processor.getMappingSource(paramName), displayName),
                            TabBarButton::ExtraComponentPlacement::afterText);
        bar.setColour(TabbedButtonBar::tabOutlineColourId, Colours::darkgrey);
        bar.setColour(TabbedButtonBar::frontOutlineColourId, Colours::darkgrey);
        for (int i = 0; i < envsAndLFOs.getTabbedButtonBar().getNumTabs(); ++i)
        {
            bar.getTabButton(i)->setAlpha(i == 0 ? 1.0f : 0.5f);
        }
    }

    tab1.addAndMakeVisible(OSCILLOSCOPE);
    //==============================================================================
    // TAB2 ========================================================================
    addAndMakeVisible(tab2);
    
    for (int i = 0; i < NUM_MACROS+1; ++i)
    {
        if (i < NUM_MACROS)
        {
            String n = "M" + String(i+1);
            if (i >= NUM_GENERIC_MACROS) n = cUniqueMacroNames[i-NUM_GENERIC_MACROS];
            macroControlLabels.add(new Label());
            macroControlLabels.getLast()->setText(n + " CC#", dontSendNotification);
            tab2.addAndMakeVisible(macroControlLabels.getLast());
        }
        
        macroControlEntries.add(new Label());
        macroControlEntries.getLast()->setEditable(true);
        macroControlEntries.getLast()->setJustificationType(Justification::centred);
        macroControlEntries.getLast()->setColour(Label::backgroundColourId,
                                                Colours::darkgrey.withBrightness(0.2f));
        macroControlEntries.getLast()->addListener(this);
        tab2.addAndMakeVisible(macroControlEntries.getLast());
        
        if (i < NUM_GENERIC_MACROS + 4)
        {
            macroControlNameLabels.add(new Label());
            macroControlNameLabels.getLast()->setText("Name", dontSendNotification);
            tab2.addAndMakeVisible(macroControlNameLabels.getLast());
            
            macroControlNames.add(new Label());
            macroControlNames.getLast()->setEditable(true);
            //macroControlNames.getLast()->setInputRestrictions(8);
            macroControlNames.getLast()->setJustificationType(Justification::centred);
            macroControlNames.getLast()->setColour(Label::backgroundColourId,
                                                   Colours::darkgrey.withBrightness(0.2f));
            macroControlNames.getLast()->addListener(this);
            tab2.addAndMakeVisible(macroControlNames.getLast());
        }
        
    }
   
    for ( int i = 0 ; i < 4; i++)
    {
        
    }
    for (int i = 0; i < MAX_NUM_VOICES+1; ++i)
    {
        String n = "String " + String(i);
        if (i == 0) n = "Global Pitch Bend & CCs";
        stringChannelLabels.add(new Label());
        stringChannelLabels.getLast()->setText(n + " Ch#", dontSendNotification);
//        stringChannelLabels.getLast()->setJustificationType(Justification::centredRight);
        tab2.addAndMakeVisible(stringChannelLabels.getLast());
        
        stringChannelEntries.add(new Label());
        stringChannelEntries.getLast()->setEditable(true);
        stringChannelEntries.getLast()->setJustificationType(Justification::centred);
        stringChannelEntries.getLast()->setColour(Label::backgroundColourId,
                                                 Colours::darkgrey.withBrightness(0.2f));
        stringChannelEntries.getLast()->addListener(this);
        tab2.addAndMakeVisible(stringChannelEntries.getLast());
    }
    
    //==============================================================================
    // TAB3 ========================================================================
    addAndMakeVisible(tab3);
    
    tab3.addAndMakeVisible(tuningTab);
    
    //==============================================================================
    
    //==============================================================================
    // TAB4 ========================================================================
    addAndMakeVisible(tab4);
    
    tab4.addAndMakeVisible(fxTab);
    MappingSource * sourcesOrganizedByID[128];
    for (auto mapModel : p.sourceMap)
    {
        MappingSource *s = new MappingSource(*this, *mapModel);
        //allSources.add(s);

        auto it = find(paramSourceOrder.begin(), paramSourceOrder.end(),s->getModel().name);
        int index = 0;
          // If element was found
          if (it != paramSourceOrder.end())
          {
              
              // calculating the index
              // of K
            index = it - paramSourceOrder.begin();
          }
        
        sourcesOrganizedByID[index] = s;
    }
    for (int j= 0; j< paramSourceOrder.size(); j++)
    {
        
        allSources.add(sourcesOrganizedByID[j]);
        tab4.addAndMakeVisible(allSources[j]);
        
    }
    //==============================================================================
    
    



    
    tabs.addTab("Synth", Colours::black, &tab1, false);
    tabs.addTab("Control", Colours::black, &tab2, false);
    
    tabs.addTab("FX", Colours::black, &tab4, false);
    
    //tabs.getTabbedButtonBar().setLookAndFeel(ElectroLookAndFeel::getInstance());
    tabs.getTabbedButtonBar().getTabButton(0)->addListener(this);
    tabs.getTabbedButtonBar().getTabButton(1)->addListener(this);
   
    tabs.getTabbedButtonBar().getTabButton(2)->addListener(this);
  

    tabs.addTab("Tuning", Colours::black, &tab3, false);
    tabs.getTabbedButtonBar().getTabButton(3)->addListener(this);


    addAndMakeVisible(tab5);
    stopProcessingToggle.setButtonText("Stop Processing");
    stopProcessingToggle.addListener(this);
    tab1.addAndMakeVisible(stopProcessingToggle);
    mpeToggle.setButtonText("MPE mode");
    tab5.addAndMakeVisible(copedentTable);
    mpeToggle.addListener(this);
    tab1.addAndMakeVisible(mpeToggle);
    tabs.addTab("Copedent", Colours::black, &tab5, false);
    tabs.getTabbedButtonBar().getTabButton(4)->addListener(this);
    addAndMakeVisible(&tabs);
   
    hamburger_button = std::make_unique<DrawableButton>("", DrawableButton::ButtonStyle::ImageFitted);
    hamburger_button->setImages(hamburger_menu_image.get());
    hamburger_button->setButtonText("button");
    
    hamburger_button->addListener(this);
    
    setSize(EDITOR_WIDTH * processor.editorScale, EDITOR_HEIGHT * processor.editorScale);
    
    //saveStateButton.onClick = [this]() {DBG("fiuclk");};
   

    addAndMakeVisible(hamburger_button.get());
    saveStateButton.setButtonText("Save");
    
    loadStateButton.setButtonText("Load");
    saveStateButton.addListener(this);
    loadStateButton.addListener(this);
        addAndMakeVisible(saveStateButton);
        addAndMakeVisible(loadStateButton);
    sendOutButton.setButtonText("Send to Device");
    sendOutButton.onClick = [this] { 
        for(auto inputs : MidiOutput::getAvailableDevices())
        {
            if (inputs.name == "Electrosteel")
            {
                
                
                sysexOut = MidiOutput::openDevice(inputs.identifier);
                
                
            }
        }
        if (sysexOut )
        {
            MidiBuffer midiMessages; 
            Array<float> data;
            
            // Parameter values
            // Order is determined in createParameterLayout
            int count = 0;
            // int myCount = 0;
            //first send a count of the number of parameters that will be sent
            data.add(paramDestOrder.size() + 2); //plus midi key values
            data.add(processor.midiKeyMax/127.0f);
            DBG(String(count++)+ ": Midi Key Max: "+ String(processor.midiKeyMax/127.0f));
            data.add(processor.midiKeyMin/127.0f);
            DBG(String(count++)+ ": Midi Key Min: "+ String(processor.midiKeyMin/127.0f));
            for (auto id : paramDestOrder)
            {
                //data.add((float)myCount++);
                //const NormalisableRange<float>& range = vts.getParameter(id)->getNormalisableRange();
                std::regex e("^PitchBend[0-9]+$|^F[0-9]+$|^K[0-9]+$");
                if (!std::regex_match(id.toStdString(), e))
                {
                    data.add(this->vts.getParameter(id)->getValue());
                    DBG(String(count++)+ ": " + id + ": "+ String(this->vts.getParameter(id)->getValue()));
                }
                else
                {
                    DBG("skipped");
                }
                
                
                
            }
            
            //mark end of parameter values
            data.add(-2.0f);
            
            //now prepare the mapping data, need to loop through the array to count how many before adding to the final array
            Array<float> tempData;
            int mapCount = 0;
            // Mappings
            DBG("Mappings");
            DBG("Name: sourceparamid, targetparamaid, scalarsource, range, slot#");
            for (auto id : paramDestOrder)
            {
                for (int t = 0; t < 3; ++t)
                {
                    String tn = id + " T" + String(t+1);
                    if (processor.targetMap.contains(tn))
                    {
                        MappingTargetModel* target = processor.targetMap[tn];
                        if (MappingSourceModel* source = target->currentSource)
                        {
                            tempData.add(processor.sourceIds.indexOf(source->name));//SourceID
                            auto it = find(paramDestOrder.begin(), paramDestOrder.end(), id);
                            int index = 0;
                            // If element was found
                            if (it != paramDestOrder.end())
                            {
                                
                                // calculating the index
                                // of K
                                index = it - paramDestOrder.begin();
                            }
                            float tempId = index + 2;
                            tempData.add(tempId);//TargetID
                            //scalarSource -- negative 1 if no source
                            DBG("Index of "  +  id  + " is " + String(tempId));
                            float scalarsource = -1.0f;
                            if (target->currentScalarSource != nullptr)
                            {
                                scalarsource = processor.sourceIds.indexOf(target->currentScalarSource->name);
                            }
                            
                            tempData.add(scalarsource);
                            float multiplier = 1.0f;
                            const NormalisableRange<float>& range = this->vts.getParameter(id)->getNormalisableRange();
                            float tempRange = target->end;
                            if (tempRange < 0.0f)
                            {
                                multiplier = -1.0f;
                                tempRange = fabsf(target->end);
                            }
                            tempRange = ((tempRange) / (range.end - range.start));
                            tempData.add(tempRange * multiplier);//Mapping range length
                            tempData.add(t);//Mapping slot
                            DBG(tn +": " + String(processor.sourceIds.indexOf(source->name))+ ", " + String(tempId)+", " + String(scalarsource)+ ", " +String(tempRange * multiplier)+
                                ", " +String(t));
                            mapCount++;
                        }
                    }
                }
            }
            
            //now send how many mappings will follow
            data.add(mapCount);
            for (int i = 0; i < tempData.size(); i++)
            {
                data.add(tempData[i]);
            }
            
            //mark end of mapping values
            data.add(-3.0f);
            
            //next things that would be useful to send:
            // string assignment midi notes
            // tunings
            // how many extra wavetables are used and which oscillators are they assigned to
            //
            //        for (int i = 0 ; i < sourceIds.size(); i++)
            //        {
            //            DBG(String(i) +  ": " + sourceIds[i]);
            //        }
            Array<uint8_t> data7bitInt;
            union uintfUnion fu;
            
            uint16_t sizeOfSysexChunk = (64 / 5) - 3;
            int dataToSend = data.size();
            uint16_t currentChunk = 0;
            uint16_t currentDataPointer = 0;
            data7bitInt.add(0); // saying it's a preset
            data7bitInt.add(processor.presetNumber); // which preset are we saving
            data7bitInt.add(17);
            data7bitInt.add(18);
            data7bitInt.add(VERSION_NUMBER_MAJOR);
            data7bitInt.add(VERSION_NUMBER_MINOR);
            for (int i = 0; i < processor.presetName.length(); i++)
            {
                data7bitInt.add((processor.presetName.toUTF8()[i] & 127)); //printable characters are in the 0-127 range
                
            }
            uint16 remainingBlanks = 14 - processor.presetName.length();
            for (uint16 i = 0; i < remainingBlanks; i++)
            {
                data7bitInt.add(32);
            }
            //MidiMessage presetMessage = ;
            
            midiMessages.addEvent(MidiMessage::createSysExMessage(data7bitInt.getRawDataPointer(), sizeof(uint8_t) * data7bitInt.size()), 0);
            
            currentChunk++;
            
            //now send the macro names (9 characters each)
            for (int i = 0; i < NUM_GENERIC_MACROS; i++)
            {
                data7bitInt.clear();
                data7bitInt.add(0); // saying it's a preset
                data7bitInt.add(processor.presetNumber); // which preset are we saving
                
                //clip macro names to 9 letters if they are longer
                int myLength = 9;
                if (processor.macroNames[i].length() < 9)
                {
                    myLength = processor.macroNames[i].length();
                }
                for (int j = 0; j < myLength; j++)
                {
                    data7bitInt.add((processor.macroNames[i].toUTF8()[j] & 127)); //printable characters are in the 0-127 range
                }
                remainingBlanks = 9 - myLength;
                for (int  j = 0; j < remainingBlanks; j++)
                {
                    data7bitInt.add(32);
                }
                //MidiMessage presetMessage = ;
                
                midiMessages.addEvent(MidiMessage::createSysExMessage(data7bitInt.getRawDataPointer(), sizeof(uint8_t) * data7bitInt.size()), 0);
                currentChunk++;
            }
            //now send the knob/joystick names (10 characters each)
            for (int i = NUM_GENERIC_MACROS; i < (NUM_GENERIC_MACROS + 4); i++)
            {
                data7bitInt.clear();
                data7bitInt.add(0); // saying it's a preset
                data7bitInt.add(processor.presetNumber); // which preset are we saving
                
                //clip macro names to 10 letters if they are longer
                int myLength = 10;
                if (processor.macroNames[i].length() < 10)
                {
                    myLength = processor.macroNames[i].length();
                }
                for (int j = 0; j < myLength; j++)
                {
                    data7bitInt.add((processor.macroNames[i].toUTF8()[j] & 127)); //printable characters are in the 0-127 range
                }
                remainingBlanks = 10 - myLength;
                for (int  j = 0; j < remainingBlanks; j++)
                {
                    data7bitInt.add(32);
                }
                //MidiMessage presetMessage = ;
                
                midiMessages.addEvent(MidiMessage::createSysExMessage(data7bitInt.getRawDataPointer(), sizeof(uint8_t) * data7bitInt.size()), 0);
                currentChunk++;
            }
            while(currentDataPointer < dataToSend)
            {
                data7bitInt.clear();
                
                data7bitInt.add(0); // saying it's a preset
                data7bitInt.add(processor.presetNumber); // which preset are we saving
                
                //data7bitInt.add(currentChunk); // whichChhunk
                uint16_t toSendInThisChunk;
                uint16_t dataRemaining = dataToSend - currentDataPointer;
                if (dataRemaining < sizeOfSysexChunk)
                {
                    toSendInThisChunk = dataRemaining;
                }
                else
                {
                    toSendInThisChunk = sizeOfSysexChunk;
                }
                
                for (int i = currentDataPointer; i < toSendInThisChunk+currentDataPointer; i++)
                {
                    fu.f = data[i];
                    data7bitInt.add((fu.i >> 28) & 15);
                    data7bitInt.add((fu.i >> 21) & 127);
                    data7bitInt.add((fu.i >> 14) & 127);
                    data7bitInt.add((fu.i >> 7) & 127);
                    data7bitInt.add(fu.i & 127);
                    
                }
                currentDataPointer = currentDataPointer + toSendInThisChunk;
                MidiMessage presetMessage = MidiMessage::createSysExMessage(data7bitInt.getRawDataPointer(), sizeof(uint8_t) * data7bitInt.size());
                
                midiMessages.addEvent(presetMessage, 0);
                
                currentChunk++;
            }
            data7bitInt.clear();
            data7bitInt.add(126); // custom command to start parsing, sysex send is finished!
            data7bitInt.add(1); // which preset did we just finish
            MidiMessage presetMessage = MidiMessage::createSysExMessage(data7bitInt.getRawDataPointer(), sizeof(uint8_t) * data7bitInt.size());
            midiMessages.addEvent(presetMessage, 0);
            
            sysexOut->sendBlockOfMessagesNow(midiMessages);
        }
            
        
 
        
    };
    
    
    
    
    
    
    streamChangesButton.setButtonText("Stream Changes");
    streamChangesButton.onClick = [this] {
        for(auto inputs : MidiOutput::getAvailableDevices())
        {
            if (inputs.name == "Electrosteel")
            {
                
                
                sysexOut = MidiOutput::openDevice(inputs.identifier);
                
                
            }
        }
        processor.streamSend = false;
        processor.streamMapping = false;
        processor.toggleStream(); };
    
    
    
    presetNameEditor.setTitle("Preset Name");
    presetNameEditor.onFocusLost = [this] {processor.setPresetName(presetNameEditor.getText());};
    presetNameEditor.setInputRestrictions(14);
    presetNameEditor.setFont(Font(Font::getDefaultMonospacedFontName(), 12, true));
    presetNameEditor.setMultiLine(true, true);
    presetNamelabel.setText("Name", dontSendNotification);
    presetNumberlabel.setText("Number", dontSendNotification);
    presetNumber.setRange(0, 60, 1);
    
    //presetNumber.setMouseDragSensitivity(12000);
    presetNumber.setSliderSnapsToMousePosition(false);
    presetNumber.onValueChange = [this] {processor.setPresetNumber(presetNumber.getValue());};
    presetNumber.setSliderStyle(Slider::SliderStyle::IncDecButtons);
    presetNumber.setIncDecButtonsMode(juce::Slider::incDecButtonsNotDraggable);
    //presetNumber.setMouseDragSensitivity(200);
    presetNumber.setTextValueSuffix(""); //EBSPECIFIC
    presetNumber.setTitle("Preset Number");
    presetNumber.setName("Preset Number");
    presetNumber.setColour(Slider::backgroundColourId, Colours::darkgrey.withBrightness(0.2f));
    presetNumber.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);
    presetNumber.setColour(Slider::textBoxTextColourId, Colours::gold.withBrightness(0.95f));
    
    
    
    
    
    addAndMakeVisible(presetNumber);
    addAndMakeVisible(presetNamelabel);
    addAndMakeVisible(presetNumberlabel);
    addAndMakeVisible(presetNameEditor);
    addAndMakeVisible(sendOutButton);
    addAndMakeVisible(streamChangesButton);
    
    constrain->setFixedAspectRatio(EDITOR_WIDTH / EDITOR_HEIGHT);
    
//    addAndMakeVisible(*resizer);
    resizer->setAlwaysOnTop(true);
    
    versionLabel.setJustificationType(Justification::centred);
    versionLabel.setBorderSize(BorderSize<int>(2));
    versionLabel.setText("v" + String(ProjectInfo::versionString), dontSendNotification);
    versionLabel.setColour(Label::ColourIds::textColourId, Colours::lightgrey);
    addAndMakeVisible(versionLabel);
    
    update();
    startTimerHz(30);//30
    presetNumber.setValue(0);
}


ElectroAudioProcessorEditor::~ElectroAudioProcessorEditor()
{
    //    masterDial->setLookAndFeel(nullptr);
    //    ampDial->setLookAndFeel(nullptr);
    //    for (int i = 0; i < NUM_MACROS; ++i)
    //    {
    //        ccDials[i]->setLookAndFeel(nullptr);
    //    }
    pitchBendSliders[0]->setLookAndFeel(nullptr); //EBSPECIFIC

    for (int i = 0; i < NUM_CHANNELS; ++i)
    {
       
        stringActivityButtons[i]->setLookAndFeel(nullptr);
    }
    
    for (int i = 0; i < NUM_GENERIC_MACROS; ++i)
    {
        macroControlNameLabels[i]->setLookAndFeel(nullptr);
        macroControlNames[i]->setLookAndFeel(nullptr);
    }
    
    for (int i = 0; i < NUM_MACROS+1; ++i)
    {
        if (i < NUM_MACROS) macroControlLabels[i]->setLookAndFeel(nullptr);
        macroControlEntries[i]->setLookAndFeel(nullptr);
    }
    
    for (int i = 0; i < MAX_NUM_VOICES+1; ++i)
    {
        stringChannelLabels[i]->setLookAndFeel(nullptr);
        stringChannelEntries[i]->setLookAndFeel(nullptr);
    }
    
    seriesLabel.setLookAndFeel(nullptr);
    parallelLabel.setLookAndFeel(nullptr);
    envsAndLFOs.setLookAndFeel(nullptr);
    
    sendOutButton.setLookAndFeel(nullptr);
    streamChangesButton.setLookAndFeel(nullptr);
    
    midiKeyRangeSlider.setLookAndFeel(nullptr);
    midiKeyMinLabel.setLookAndFeel(nullptr);
    midiKeyMaxLabel.setLookAndFeel(nullptr);
    
    randomValueLabel.setLookAndFeel(nullptr);
    
    numVoicesLabel.setLookAndFeel(nullptr);
    numVoicesSlider.setLookAndFeel(nullptr);
    transposeLabel.setLookAndFeel(nullptr);
    transposeSlider.setLookAndFeel(nullptr);
    rangeSlider.setLookAndFeel(nullptr);
    sliderAttachments.clear();
    buttonAttachments.clear();
    fxTab.setLookAndFeel(nullptr);

}

//==============================================================================
void ElectroAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //    g.setGradientFill(ColourGradient(Colour(25, 25, 25), juce::Point<float>(0,0), Colour(10, 10, 10), juce::Point<float>(0, getHeight()), false));
    g.setColour(Colours::black);
    g.fillRect(0, 0, getWidth(), getHeight());
    
    //    Rectangle<float> panelArea = getLocalBounds().toFloat();
    //    panelArea.reduce(getWidth()*0.025f, getHeight()*0.01f);
    //    panelArea.removeFromBottom(getHeight()*0.03f);
    //    panel->drawWithin(g, panelArea, RectanglePlacement::centred, 1.0f);
    
    g.fillRect(getWidth() * 0.25f, getHeight() * 0.25f, getWidth() * 0.6f, getHeight() * 0.5f);
    g.fillRect(getWidth() * 0.25f, getHeight() * 0.75f, getWidth() * 0.2f, getHeight() * 0.15f);
}

void ElectroAudioProcessorEditor::resized()
{

    int width = getWidth();
    int height = getHeight();
    
    float s = width / EDITOR_WIDTH;
    
    processor.editorScale = 1.05f;
    // Set OSCILLOSCOPE bounds

    //tabs.setBounds(getLocalBounds().expanded(1));
    
    tabs.setBoundsRelative(0,0,1,1.0);
    tabs.setTabBarDepth(30*s);
    
    height -= tabs.getTabBarDepth();
    
    
    
    //==============================================================================
    // TAB1 ========================================================================
    for (int i = 0; i < NUM_OSCS; ++i)
    {
        oscModules[i]->setBounds(-1, (120*s*i)-i-1, 540*s+1, 120*s);
    }


    envsAndLFOs.setBounds(-1, oscModules.getLast()->getBottom()-1, 540*s+1, 160*s);
    envsAndLFOs.setIndent(10*s);
    envsAndLFOs.setTabBarDepth(25*s);
    
    noiseModule->setBounds(oscModules[0]->getRight()-1, -1, 364*s, 121*s-2);
    
    //ettingsComponent.setBounds(noiseModule->getRight()-1, -1, 114*s, 100*s-1);
    
    seriesParallelComponent.setBounds(540*s-1, noiseModule->getBottom()-1, 364*s, 40*s);
    seriesLabel.setBounds(0, 0, 60*s, 22*s);
    parallelLabel.setBounds(302*s, 0, 60*s, 22*s);
    seriesParallelSlider.setBounds(0, 18*s, 363*s, 22*s);
    
    filterModules[0]->setBounds(540*s-1, seriesParallelComponent.getBottom()-1, 360*s+2, 124*s);
    for (int i = 1; i < NUM_FILT; ++i)
    {
        filterModules[i]->setBounds(540*s-1, filterModules[i-1]->getBottom()-1, 360*s+2, 124*s);
    }
    
    outputModule->setBounds(540*s-1, filterModules.getLast()->getBottom()-1, 360*s+2, 114*s);
    
    const float knobSize = 40.0f*s;
    for (int i = 0; i < NUM_GENERIC_MACROS; ++i)
    {
        macroDials[i]->setBounds(6*s + (knobSize+2)*i, 523*s, knobSize, knobSize*1.8f);
    }
    
    
    uniqueMacroComponent.setBounds(6*s + (knobSize+2)*NUM_GENERIC_MACROS + 3,
                                   outputModule->getBottom()-1, 700, 69);
    for (int i = NUM_GENERIC_MACROS; i < NUM_MACROS; ++i)
    {
        macroDials[i]->setBounds(6*s + (knobSize *.92f)*(i-NUM_GENERIC_MACROS) - 3,
                                 523*s - uniqueMacroComponent.getY(),
                                 knobSize*.9f,
                                 knobSize*1.8f);
    }
    
    
    int align = 78*s;
    int x = 900*s - 10*align;
    int y = 582*s;
    
    midiKeyComponent.setBounds(-1, y-2, 610, 33);//657
    midiKeySource->setBounds(5, 7, x-40, 22*s - 4);
    midiKeyMinLabel.setBounds(midiKeySource->getRight()+4, 7, 40, 22*s - 4);
    midiKeyRangeSlider.setBounds(midiKeyMinLabel.getRight(), 7, 420, 22*s - 4);
    midiKeyMaxLabel.setBounds(midiKeyRangeSlider.getRight(), 7, 40, 22*s - 4);


        
    int r = (10*align) % 12;
    int w = (10*align) / 12;
    y = height-35*s+2;//
    
    stringActivityButtons[0]->setBounds(0, y, w, 35*s);
    pitchBendSliders[0]->setBounds(0,
                                   midiKeyComponent.getBottom()-1,
                                   w + (r > 0 ? 1 : 0), 27*s); //EBSPECIFIC
    rangeLabel.setBounds(pitchBendSliders[0]->getRight(), midiKeyComponent.getBottom()-1,w + (r > 0 ? 1 : 0), 12*s);
        rangeSlider.setBounds(pitchBendSliders[0]->getRight(), stringActivityButtons[0]->getY() -17*s ,w + (r > 0 ? 1 : 0), 17*s);
        
        numVoicesLabel.setBounds(rangeSlider.getRight(), midiKeyComponent.getBottom()-1, w + (r > 0 ? 1 : 0), 12*s);
        numVoicesSlider.setBounds(rangeSlider.getRight(), stringActivityButtons[0]->getY() -17*s, w + (r > 0 ? 1 : 0), 17*s);
        transposeLabel.setBounds(numVoicesSlider.getRight(), midiKeyComponent.getBottom()-1, w + (r > 0 ? 1 : 0), 12*s);
        transposeSlider.setBounds(numVoicesSlider.getRight(), stringActivityButtons[0]->getY() -17*s, w + (r > 0 ? 1 : 0), 17*s);
    velocityComponent.setBounds(transposeSlider.getRight() + 42, midiKeyComponent.getBottom()-1, 102, 33);
    velocitySource->setBounds(5, 7, 91, 22 * s - 4);

    randomComponent.setBounds(velocityComponent.getRight() - 1, midiKeyComponent.getBottom()-1, x + 60, 33);
    randomSource->setBounds(5, 7, x+2, 22*s - 4);
    randomValueLabel.setBounds(randomSource->getRight()+4, 7, 40, 22*s - 4);
    for (int i = 1; i < 11; ++i)
    {
        if( i < 6)
            stringActivityButtons[i]->setBounds(stringActivityButtons[i-1]->getRight(), y,
                                            4 * align/12, 18*s);
        else
            stringActivityButtons[i]->setBounds(stringActivityButtons[i-6]->getRight(), y+ 15*s,
                                            4 * align/12, 18*s);
    }
    muteToggle.setBounds(stringActivityButtons[5]->getRight(), stringActivityButtons[5]->getY(), 1.5f * align, 18*s);
   
    pedalControlsMasterToggle.setBounds(stringActivityButtons[10]->getRight(), stringActivityButtons[10]->getY(),1.5f *align,18*s);
    mpeToggle.setBounds(pedalControlsMasterToggle.getRight(), pedalControlsMasterToggle.getY(), align, 18*s );
    stopProcessingToggle.setBounds(mpeToggle.getRight(), mpeToggle.getY(), 1.5f * align, 18*s);
    //OSCILLOSCOPE.get
    //    keyboard.setBoundsRelative(0.f, 0.86f, 1.0f, 0.14f);
    //    keyboard.setKeyWidth(width / 52.0f);
    
   
    
    OSCILLOSCOPE.setBoundsRelative(0.85,0.87,0.15, 0.13 );
    //meters.setBounds(540*s-1, outputModule->getBottom()-1, 360*s+2, 114*s);
    setVerticalRotatedWithBounds(meters, true, Rectangle<int>(540*s+ 45, macroDials.getFirst()->getBottom() - 10, 190*s+2, 50*s));
    //meters.setBounds(540*s-1, outputModule->getBottom()-1, 360*s+2, 114*s);
    masterSlider.setBounds(540*s+ 45, muteToggle.getY(), 190*s - 5, mpeToggle.getHeight());
//    setVerticalRotatedWithBounds(meters, true, Rectangle<int>(540*s+100, outputModule->getBottom()-1, 300*s+2, 60*s));
    //==============================================================================
    // TAB2 ========================================================================
    
    x = 40;
    y = 40;
    int h = 30;
    int pad = 4;
    for (int i = 0; i < NUM_GENERIC_MACROS + 4; ++i)
    {
        int padx = (i/6) < 3 ? 0 : 2;
        if (i < (NUM_GENERIC_MACROS + 4))
        {
            macroControlNameLabels[i]->setBounds(x + 216*(i/12) + padx,
                                                 y+(h+pad)*(i%12), 70, h);
            macroControlNames[i]->setBounds(macroControlNameLabels[i]->getRight(),
                                            macroControlNameLabels[i]->getY(), 130, h);
        }
    }
    for (int i = 0; i < NUM_MACROS; ++i)
    {
        int padx = (i/6) < 3 ? 0 : 2;
        macroControlLabels[i]->setBounds(x + 216*((i+12)/12) + padx, y+(h+pad)*(i%12), 100, h);
        macroControlEntries[i]->setBounds(macroControlLabels[i]->getRight(),
                                          macroControlLabels[i]->getY(), 100, h);
      
    }
    
    
    y = 460;
    stringChannelLabels[0]->setBounds(x, y, 300, h);
    stringChannelEntries[0]->setBounds(stringChannelLabels[0]->getRight(),
                                       stringChannelLabels[0]->getY(), 100, h);
    for (int i = 1; i < MAX_NUM_VOICES+1; ++i)
    {
        stringChannelLabels[i]->setBounds(x + 300*((i-1)/4),
                                          y+(h+pad)+(h+pad)*((i-1)%4), 150, h);
        stringChannelEntries[i]->setBounds(stringChannelLabels[i]->getRight(),
                                           stringChannelLabels[i]->getY(), 100, h);
    }
 
    
    //==============================================================================
    // TAB3 ========================================================================
    
    tuningTab.setBoundsRelative(0.05f, 0.08f, 0.9f, 0.84f);
    
    //==============================================================================
    
    //versionLabel.setBounds(width*0.79f, 0, width * 0.05f, tabs.getTabBarDepth());
    //versionLabel.setFont(euphemia.withHeight(20*s));
    sendOutButton.setBounds(width*0.82f, -1, width*0.08f+2, tabs.getTabBarDepth());
    streamChangesButton.setBounds(width*0.90f, -1, width*0.1f+2, tabs.getTabBarDepth());
    presetNumber.setBounds(sendOutButton.getX() - width*0.08f+2, -1, width*0.07f+1, tabs.getTabBarDepth());
    
//    presetNamelabel.setBounds(presetNameEditor.getX()-  width*0.05f+2, tabs.getTabBarDepth()/2,width*0.05f+2, tabs.getTabBarDepth()/2);
    presetNumberlabel.setBounds(presetNumber.getX()-  width*0.05f+2, -1,width*0.05f+2, tabs.getTabBarDepth()/2);
    presetNameEditor.setBounds(presetNumberlabel.getX() - presetNumberlabel.getWidth() -10, -1, width*0.06f+2, tabs.getTabBarDepth());
    presetNamelabel.setBounds(presetNameEditor.getX() - presetNameEditor.getWidth()/2 - 10, -1,width*0.05f+2, tabs.getTabBarDepth() /2);
    hamburger_button->setBounds(tabs.getTabbedButtonBar().getTabButton(4)->getRight(), tabs.getTabbedButtonBar().getTabButton(4)->getY(), 30, 30);
    int logoLeft = hamburger_button->getRight();
    Rectangle<float> logoArea (logoLeft, 0, 98*s, tabs.getTabBarDepth());
    logo->setTransformToFit (logoArea,
                             RectanglePlacement::xLeft +
                             RectanglePlacement::yTop +
                             RectanglePlacement::fillDestination);
   
    synderphonicsLabel.setBounds(logoLeft+50*s, -5*s, 150*s, 34*s);
    synderphonicsLabel.setFont(euphemia.withHeight(34*s));
//    if(!JUCEApplicationBase::isStandaloneApp())
//    {
//        synderphonicsLabel.setBounds(logoLeft+50*s, -5*s, 100*s, 34*s);
//        synderphonicsLabel.setFont(euphemia.withHeight(34*s));
    
    
    
        //saveStateButton.setBounds(synderphonicsLabel.getRight(), synderphonicsLabel.getY(), 50*s, 15*s);
        //loadStateButton.setBounds(synderphonicsLabel.getRight(), saveStateButton.getBottom(), 50*s, 15*s);
//    }
//    ElectrobassLabel.setBounds(synderphonicsLabel.getRight(), -5*s, 300*s, 34*s);
//    ElectrobassLabel.setFont(euphemia.withHeight(34*s).withStyle(3));
    
    float rt = EDITOR_WIDTH / EDITOR_HEIGHT;
    constrain->setSizeLimits(200, 200/rt, 800*rt, 800);
    resizer->setBounds(getWidth()-12, getHeight()-12, 12, 12);
    
    //    container.setBounds(getLocalBounds());
    
    //==============================================================================
    // TAB3 ========================================================================
    
    fxTab.setBoundsRelative(0.05f, 0.08f, 0.99f, 0.75f);
    for (int i = 0; i < allSources.size(); ++i)
    {
        allSources[i]->setBounds(6*s + (93+2)*(i/5), fxTab.getBottom() + ((i%5)*(48*0.5f)), 93, 38*0.5f);
    }
    //==============================================================================
  
    // TAB5
    
    copedentTable.setBoundsRelative(0.02f, 0.04f, 0.9f, 0.9f);
    
   
}

void ElectroAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == nullptr) return;

    if (slider == &midiKeyRangeSlider)
    {
        updateMidiKeyRangeSlider(midiKeyRangeSlider.getMinValue(),
                                 midiKeyRangeSlider.getMaxValue());
        if(processor.stream)
        {
            processor.streamID1 = 0;
            processor.streamID2 = 1;
            processor.streamValue1 = midiKeyRangeSlider.getMinValue();
            processor.streamValue2 = midiKeyRangeSlider.getMaxValue();
            float arr[4] = {0, (float)midiKeyRangeSlider.getMinValue(), 1, (float)midiKeyRangeSlider.getMaxValue()};
            knobQueue.writeTo( arr, 4);
        }
    }
    else if (slider == &numVoicesSlider)
    {
        updateNumVoicesSlider(numVoicesSlider.getValue()); //EBSPECIFIC
    }
    
    else if (slider == &rangeSlider)
    {
        //vts.getParameter("PitchBendRange")->setValue(rangeSlider.getValue());
        processor.pitchBendRange->end = rangeSlider.getValue();
        processor.pitchBendRange->start = -rangeSlider.getValue();
        processor.pitchBendRange->setSkewForCentre(0.0f);
        //pitchBendSliders[0]->setRange(- (rangeDownSlider.getValue()), rangeUpSlider.getValue());
        if(processor.stream)
        {
            //processor.streamID1 = 0;
            //processor.streamID2 = 1;
            processor.streamValue1 = rangeSlider.getValue();
            auto it = find(paramDestOrder.begin(), paramDestOrder.end(),"PitchBendRange" );
            int index = 0;
              // If element was found
              if (it != paramDestOrder.end())
              {
                  
                  // calculating the index
                  // of K
                index = it - paramDestOrder.begin();
              }
            int tempId = index + 2;
            processor.streamID1 = tempId;
            DBG("Send: PitchBendRange with id" + String(tempId) +
                " as " + String(processor.streamValue1) );
            
            float arr[2] = {(float)tempId, (float)rangeSlider.getValue()};
            knobQueue.writeTo( arr, 2);
            
        }
    } else if (slider == &seriesParallelSlider)
    {
        if(processor.stream)
        {
            processor.streamValue1 = slider->getValue();
            
            String tempString = slider->getName();
            auto it = find(paramDestOrder.begin(), paramDestOrder.end(), slider->getName());
            int index = -1;
            // If element was found
            if (it != paramDestOrder.end())
            {
                
                // calculating the index
                // of K
                index = it - paramDestOrder.begin();
            }

            int tempId = index + 2;
            processor.streamID1 = tempId;
            //button->get
            DBG("Send: " + slider->getName() + " with ID"  + String(tempId) + " and value " + String(processor.streamValue1)/*String(streamValue)*/);
            float arr[2] = {(float)tempId, (float)slider->getValue()};
            knobQueue.writeTo( arr, 2);
        
        }
    }
    else if (pitchBendSliders.contains(slider))
    {
        
    }
//    if ( processor.stream)
//    {
//        processor.streamValue1 = slider->getValue();
//       String tempString = slider->getName();
//        auto it = find(paramDestOrder.begin(), paramDestOrder.end(), slider->getName());
//        int index = -1;
//          // If element was found
//          if (it != paramDestOrder.end())
//          {
//              
//              // calculating the index
//              // of K
//            index = it - paramDestOrder.begin();
//          }
//        if ((index != -1) && ((index < 1) || (index > 12))) // to avoid sending things not in the dest array (like foot pedals and knee levers)
//        {
//            int tempId = index + 2;
//            processor.streamID1 = tempId;
//            //button->get
//            DBG("Send: " + slider->getName() + " with ID"  + String(tempId) + " and value " + String(processor.streamValue1)/*String(streamValue)*/);
//            float arr[2] = {(float)tempId, (float)slider->getValue()};
//            knobQueue.writeTo( arr, 2);
//        }
    else if (slider == &transposeSlider)
    {
        if(processor.stream)
        {
            processor.streamValue1 = (slider->getValue() + 48.0f) * 0.010416666666667;
            /*String tempString = slider->getName();
            auto it = find(paramDestOrder.begin(), paramDestOrder.end(), slider->getName());
            int index = -1;
            // If element was found
            if (it != paramDestOrder.end())
            {
                
                // calculating the index
                // of K
                index = it - paramDestOrder.begin();
            }
             */
            int index =(slider->getComponentID().getIntValue());
            int tempId = 14 + 2;
            processor.streamID1 = tempId;
            //button->get
            DBG("Send: " + slider->getName() + " with ID"  + String(tempId) + " and value " + String(processor.streamValue1)/*String(streamValue)*/);
            float arr[2] = {(float)tempId, (float)processor.streamValue1};
            knobQueue.writeTo( arr, 2);
        }
    }
    else if (slider == &masterSlider)
    {
        if(processor.stream)
        {
            processor.streamValue1 = slider->getValue();
            String tempString = slider->getName();
            auto it = find(paramDestOrder.begin(), paramDestOrder.end(), slider->getName());
            int index = -1;
            // If element was found
            if (it != paramDestOrder.end())
            {
                
                // calculating the index
                // of K
                index = it - paramDestOrder.begin();
            }

            int tempId = index + 2;
            processor.streamID1 = tempId;
            //button->get
            DBG("Send: " + slider->getName() + " with ID"  + String(tempId) + " and value " + String(processor.streamValue1)/*String(streamValue)*/);
            float arr[2] = {(float)tempId, (float)slider->getValue()};
            knobQueue.writeTo( arr, 2);
        }
    }
    else { //macros
        //DBG(String(vts.getParameter(slider->getName())->getValue()));
        if(std::count(cUniqueMacroNames.begin(), cUniqueMacroNames.end(),slider->getName()))
        {
            auto it = find(cUniqueMacroNames.begin(), cUniqueMacroNames.end(),slider->getName() );
            int index = 0;
              // If element was found
              if (it != cUniqueMacroNames.end())
              {
                  
                  // calculating the index
                  // of K
                index = it - cUniqueMacroNames.begin();
              }
            //DBG(String(index + (NUM_GENERIC_MACROS -1)));
            processor.ccSources.getUnchecked(index + (NUM_GENERIC_MACROS ))->setValue(slider->getValue());
        } else
        {
            
            processor.ccSources.getUnchecked(slider->getName().substring(1).getIntValue() - 1)->setValue(slider->getValue());
        }
        
    }
      
   
}

void ElectroAudioProcessorEditor::fadersChanged (std::vector<float> faderValues)
{
    vts.getParameter("Master")->setValueNotifyingHost(faderValues[0]);
}

void ElectroAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == nullptr) return;

    if (ToggleButton* tb = dynamic_cast<ToggleButton*>(button))
    {
        if (tb == &stopProcessingToggle)
        {
            processor.setProcessing(!tb->getToggleState());
        }
        if (tb == &mpeToggle)
        {
            updateMPEToggle(tb->getToggleState());
        }
        
        if (tb == &muteToggle)
        {
            processor.setMute(tb->getToggleState());
        }
        if (tb == &pedalControlsMasterToggle)
        {
            processor.pedalControlsMaster =pedalControlsMasterToggle.getToggleState();
            if (processor.stream)
            {
                processor.streamValue1 = vts.getParameter(button->getName())->getValue();
                auto it = find(paramDestOrder.begin(), paramDestOrder.end(),button->getName() );
                int index = 0;
                  // If element was found
                  if (it != paramDestOrder.end())
                  {
                      
                      // calculating the index
                      // of K
                    index = it - paramDestOrder.begin();
                  }
                int tempId = index + 2;
                processor.streamID1 = tempId;
                //button->get
                DBG("Send: " + button->getName() + " with ID"  + String(tempId) + " and value " + String(processor.streamValue1));
                float arr[2] = {(float)tempId, vts.getParameter(button->getName())->getValue()};
                knobQueue.writeTo( arr, 2);
            }
        }
    }
    
    if (button == tabs.getTabbedButtonBar().getTabButton(0))
    {
        //tab1.addAndMakeVisible(mpeToggle);
        tab1.addAndMakeVisible(OSCILLOSCOPE);
        for (auto slider : pitchBendSliders) tab1.addAndMakeVisible(slider);
        for (auto button : stringActivityButtons) tab1.addAndMakeVisible(button);
    }
    else if (button == tabs.getTabbedButtonBar().getTabButton(1))
    {
        //tab2.addAndMakeVisible(mpeToggle);
        for (auto slider : pitchBendSliders) tab2.addAndMakeVisible(slider);
        for (auto button : stringActivityButtons) tab2.addAndMakeVisible(button);
    }
    else if (button == tabs.getTabbedButtonBar().getTabButton(2))
    {
        tab4.addAndMakeVisible(OSCILLOSCOPE);
    }
    else if (button == tabs.getTabbedButtonBar().getTabButton(3))
    {
        
    }
    else if (button == tabs.getTabbedButtonBar().getTabButton(4))
    {
        //tab5.addAndMakeVisible(mpeToggle);
    } else if (button == &saveStateButton)
    {
        stateFileChooser = std::make_unique<FileChooser> (TRANS("Save current state"),
                                                          getLastFile());
        auto flags = FileBrowserComponent::saveMode
                   | FileBrowserComponent::canSelectFiles
                   | FileBrowserComponent::warnAboutOverwriting;

        stateFileChooser->launchAsync (flags, [this] (const FileChooser& fc)
                                       {
            if (fc.getResult() == File{})
                return;
            
            setLastFile (fc);
            
            MemoryBlock data;
            processor.getStateInformation (data);
            
            if (! fc.getResult().replaceWithData (data.getData(), data.getSize()))
                AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                  TRANS("Error whilst saving"),
                                                  TRANS("Couldn't write to the specified file!"));
        });
    } else if (button == &loadStateButton)
    {
        
    }
    else if (button == hamburger_button.get())
    {
        getHamburgerMenu().showMenuAsync (PopupMenu::Options().withTargetComponent (hamburger_button.get()),
                                          [this](int result){
            if(this == nullptr)
            {
                PopupMenu::dismissAllActiveMenus();
                return;
            }
            
            if( result == 1)
            {
                stateFileChooser = std::make_unique<FileChooser> (TRANS("Load a saved state"),
                                                                  getLastFile());
                auto flags = FileBrowserComponent::openMode
                           | FileBrowserComponent::canSelectFiles;

                stateFileChooser->launchAsync (flags, [this] (const FileChooser& fc)
                {
                    if (fc.getResult() == File{})
                        return;

                    setLastFile (fc);

                    MemoryBlock data;
                    
                    if (fc.getResult().getFileExtension() == ".ebp")
                    {
                        
                        int presetNumber = fc.getResult().getFileName().substring(0,2).getIntValue();
                        if (fc.getResult().loadFileAsData (data))
                            processor.setStateEBP(data.getData(), (int) data.getSize(),presetNumber);
                        
                    }

                    if (fc.getResult().loadFileAsData (data))
                        processor.setStateInformation (data.getData(), (int) data.getSize());
                    else
                        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                          TRANS("Error whilst loading"),
                                                          TRANS("Couldn't read from the specified file!"));
                });
            }
            else if (result == 2)
            {
                stateFileChooser = std::make_unique<FileChooser> (TRANS("Save current state"),
                                                                  getLastFile());
                auto flags = FileBrowserComponent::saveMode
                           | FileBrowserComponent::canSelectFiles
                           | FileBrowserComponent::warnAboutOverwriting;

                stateFileChooser->launchAsync (flags, [this] (const FileChooser& fc)
                                               {
                    if (fc.getResult() == File{})
                        return;
                    
                    setLastFile (fc);
                    
                    MemoryBlock data;
                    processor.getStateInformation (data);
                    
                    if (! fc.getResult().replaceWithData (data.getData(), data.getSize()))
                        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                                          TRANS("Error whilst saving"),
                                                          TRANS("Couldn't write to the specified file!"));
                });
            }
            else if (result == 3)
            {
                int size;
                String xmlData = CharPointer_UTF8(BinaryData::getNamedResource("default_xml", size ));
                processor.setStateInformation(XmlDocument::parse(xmlData).get());
            }

        });
    }
}

void ElectroAudioProcessorEditor::labelTextChanged(Label* label)
{
    if (label == nullptr) return;
    
    if (label == &midiKeyMinLabel || label == &midiKeyMaxLabel)
    {
        updateMidiKeyRangeSlider(midiKeyMinLabel.getText().getIntValue(),
                                 midiKeyMaxLabel.getText().getIntValue());
    }
    else if (macroControlEntries.contains(label))
    {
        int ctrl = label->getText().getIntValue();
        updateMacroControl(macroControlEntries.indexOf(label), ctrl);
    }
    else if (macroControlNames.contains(label))
    {
        String name = label->getText().substring(0, 10);
        updateMacroNames(macroControlNames.indexOf(label), name);
    }
    else if (stringChannelEntries.contains(label))
    {
        int ch = label->getText().getIntValue();
        updateStringChannel(stringChannelEntries.indexOf(label), ch);
    }
}

void ElectroAudioProcessorEditor::mouseDown (const MouseEvent &event)
{
    if (MappingSource* ms = dynamic_cast<MappingSource*>(event.originalComponent->getParentComponent()))
    {
        currentMappingSource = ms;
        if (TabBarButton* tbb = dynamic_cast<TabBarButton*>(ms->getParentComponent()))
        {
            if (&ms->button != event.originalComponent)
            {
                tbb->triggerClick();
            }
        }
        startDragging(ms->getName(), ms);
    }
}



bool ElectroAudioProcessorEditor::keyPressed (const KeyPress &key, Component *originatingComponent)
{
    return false;
}

void ElectroAudioProcessorEditor::timerCallback()
{
    for (int i = 0; i < MAX_NUM_VOICES+1; ++i)
    {
//        stringActivityButtons[i]->setToggleState(processor.stringIsActive(i),
//
            stringActivityButtons[i]->setAlpha(processor.stringIsActive(i) ?1.f :  0.5f  );
        
    }
    updateRandomValueLabel(processor.lastRandomValue);
    // Loop through all meters (channels)...
    for (int meterIndex = 0; meterIndex < meters.getNumChannels(); ++meterIndex)
    {
       // Get the level, of the specified meter (channel), from the audio processor...
       meters.setInputLevel (meterIndex, processor.getPeakLevel (meterIndex));
    }

    meters.refresh(true);
    
    if (processor.stream)
    {
        sysexMidi.clear();
        //knobMidiArray.clearQuick();
        int size = knobQueue.writeToArray(knobMidiArray);
        String outString;
        for(int i = 0; i < size; i+=2)
        {
            union uintfUnion fu;
            Array<uint8_t> data7bitInt;
            data7bitInt.add(3);
            data7bitInt.add(0);
            fu.f = knobMidiArray[i];
            data7bitInt.add((fu.i >> 28) & 15);
            data7bitInt.add((fu.i >> 21) & 127);
            data7bitInt.add((fu.i >> 14) & 127);
            data7bitInt.add((fu.i >> 7) & 127);
            data7bitInt.add(fu.i & 127);
            
            fu.f = LEAF_clip(0.0f, knobMidiArray[i+1], 1.0f);
            data7bitInt.add((fu.i >> 28) & 15);
            data7bitInt.add((fu.i >> 21) & 127);
            data7bitInt.add((fu.i >> 14) & 127);
            data7bitInt.add((fu.i >> 7) & 127);
            data7bitInt.add(fu.i & 127);
            
            
            sysexMidi.addEvent(MidiMessage::createSysExMessage(data7bitInt.getRawDataPointer(), sizeof(uint8_t) * data7bitInt.size()), 0);
            data7bitInt.clear();
            
            data7bitInt.add(126);
            sysexMidi.addEvent(MidiMessage::createSysExMessage(data7bitInt.getRawDataPointer(), sizeof(uint8_t) * data7bitInt.size()), 0);
            DBG("MIDI SEND: Add Id" + String(knobMidiArray[i]) + "with value" + String(knobMidiArray[i+1]));
            
        }
        
        
        size = mappingQueue.writeToArray(knobMidiArray);
        
        for(int i = 0; i < size; i+=4)
        {
            union uintfUnion fu;
            Array<uint8_t> data7bitInt;
            data7bitInt.add(4);
            data7bitInt.add(0);
            
            fu.f = (float)knobMidiArray[0];
            data7bitInt.add((fu.i >> 28) & 15);
            data7bitInt.add((fu.i >> 21) & 127);
            data7bitInt.add((fu.i >> 14) & 127);
            data7bitInt.add((fu.i >> 7) & 127);
            data7bitInt.add(fu.i & 127);
            
            data7bitInt.add(knobMidiArray[1]);
            data7bitInt.add(knobMidiArray[2]);
            
            fu.f = (float)knobMidiArray[3];
            data7bitInt.add((fu.i >> 28) & 15);
            data7bitInt.add((fu.i >> 21) & 127);
            data7bitInt.add((fu.i >> 14) & 127);
            data7bitInt.add((fu.i >> 7) & 127);
            data7bitInt.add(fu.i & 127);
            
            sysexMidi.addEvent(MidiMessage::createSysExMessage(data7bitInt.getRawDataPointer(), sizeof(uint8_t) * data7bitInt.size()), 0);
            data7bitInt.clear();
            data7bitInt.add(126);
            sysexMidi.addEvent(MidiMessage::createSysExMessage(data7bitInt.getRawDataPointer(), sizeof(uint8_t) * data7bitInt.size()), 0);
            
            DBG("MIDI SEND: Add Id" + String(knobMidiArray[i]) + "with slot" + String(knobMidiArray[i+1]) + "with changetype" + String(knobMidiArray[i+2])+ "with value" + String(knobMidiArray[i+3]));
        }
    }
    if (sysexOut)
    {
        sysexOut->sendBlockOfMessagesNow(sysexMidi);
    }
    else
    {
        sysexMidi.clear();
    }

}

void ElectroAudioProcessorEditor::update()
{
    updateMPEToggle(processor.getMPEMode());
    for (int i = 0; i < MAX_NUM_VOICES+1; ++i)
    {
        updateStringChannel(i, processor.stringChannels[i]);
    }
    for (int i = 0; i < NUM_MACROS+1; ++i)
    {
        updateMacroControl(i, processor.macroCCNumbers[i]);
    }
    for (int i = 0; i < NUM_GENERIC_MACROS + 4; ++i)
    {
        updateMacroNames(i, processor.macroNames[i]);
    }
    updateMidiKeyRangeSlider(processor.midiKeyMin, processor.midiKeyMax);
    updateNumVoicesSlider(processor.numVoicesActive);
    updateRandomValueLabel(processor.lastRandomValue);

    outputModule->updateFXOrder(processor.fxPost->load());

    for(auto osc : oscModules)
    {
        osc->updateShapeCB();
    }
    presetNameEditor.setText(processor.getPresetName());
    presetNumber.setValue(processor.getPresetNumber());
}


void ElectroAudioProcessorEditor::updateMPEToggle(bool state)
{
    processor.setMPEMode(state);
    mpeToggle.setToggleState(state, dontSendNotification);
    for (int i = 1; i < pitchBendSliders.size(); ++i)
    {
        pitchBendSliders[i]->setAlpha(state ? 1.f : 0.7f);
        pitchBendSliders[i]->setEnabled(state);
        pitchBendSliders[i]->setValue(0.f);
    }
    for (int i = 0; i < stringChannelEntries.size(); ++i)
    {
        stringChannelEntries[i]->setAlpha(state ? 1.f : 0.7f);
        String text = "All";
        if (state) text = String(processor.stringChannels[i]);
        stringChannelEntries[i]->setText(text, dontSendNotification);
        stringChannelEntries[i]->setEnabled(state);
    }
}

void ElectroAudioProcessorEditor::updatePedalVolumeControl(bool state)
{
    pedalControlsMasterToggle.setToggleState(state, sendNotification);
}



void ElectroAudioProcessorEditor::updateStringChannel(int string, int ch)
{
    ch = jlimit(0, 16, ch);
    // Handle mapping that will be overwritten
    for (int i = 0; i < MAX_NUM_VOICES+1; ++i)
    {
        if (processor.stringChannels[i] == ch)
        {
            processor.stringChannels[i] = 0;
            stringChannelEntries[i]->setText("", dontSendNotification);
        }
    }
    // Disable the old mapping
    processor.channelToStringMap.set(processor.stringChannels[string], -1);
    // Set the new mapping
    processor.stringChannels[string] = ch;
    processor.channelToStringMap.set(ch, string);
    // Update the text
    bool state = processor.getMPEMode();
    String text = "All";
    if (state) text = String(processor.stringChannels[string]);
    stringChannelEntries[string]->setAlpha(state ? 1.f : 0.7f);
    stringChannelEntries[string]->setText(text, dontSendNotification);
    stringChannelEntries[string]->setEnabled(state);
}

void ElectroAudioProcessorEditor::updateMacroControl(int macro, int ctrl)
{
    ctrl = jlimit(0, 127, ctrl);
    // Handle mapping that will be overwritten
    for (int i = 0; i < NUM_MACROS+1; ++i)
    {
        if (processor.macroCCNumbers[i] == ctrl)
        {
            processor.macroCCNumbers[i] = 0;
            macroControlEntries[i]->setText("", dontSendNotification);
        }
    }
    // Disable the old mapping
    processor.ccNumberToMacroMap.set(processor.macroCCNumbers[macro], -1);
    // Set the new mapping
    processor.macroCCNumbers[macro] = ctrl;
    processor.ccNumberToMacroMap.set(ctrl, macro);
    // Update the text
    String text = ctrl > 0 ? String(processor.macroCCNumbers[macro]) : "";
    macroControlEntries[macro]->setText(text, dontSendNotification);
}

void ElectroAudioProcessorEditor::updateMacroNames(int macro, String name)
{
    processor.macroNames.set(macro, name);
    macroControlNames[macro]->setText(processor.macroNames[macro], dontSendNotification);
}

void ElectroAudioProcessorEditor::updateMidiKeyRangeSlider(int min, int max)
{
    processor.midiKeyMin = min;
    processor.midiKeyMax = max;
    midiKeyRangeSlider.setMinAndMaxValues(processor.midiKeyMin, processor.midiKeyMax,
                                          dontSendNotification);
    midiKeyMinLabel.setText(String(processor.midiKeyMin), dontSendNotification);
    midiKeyMaxLabel.setText(String(processor.midiKeyMax), dontSendNotification);
}

void ElectroAudioProcessorEditor::updateNumVoicesSlider(int numVoices)
{
    //processor.numVoicesActive = numVoices;
    processor.setNumVoicesActive(numVoices);
    numVoicesSlider.setValue(numVoices, dontSendNotification);
    for (int i = 0; i < MAX_NUM_VOICES; ++i)
    {
        tSimplePoly_allNotesOff(processor.strings[i]);
        for (auto e : processor.envs) e->noteOff(i, 0);
    }


    for (int v = 0; v < processor.numVoicesActive; v++)
    {
        for (int i = 0; i < NUM_OSCS; i++)
        {
            processor.oscs[i]->loadAll(v);
        }
        for (int i = 0; i < NUM_ENVS; i++)
        {
            processor.envs[i]->loadAll(v);
        }
        processor.noise->loadAll(v);
        for (int i = 0; i < NUM_FILT; i++)
        {
            processor.filt[i]->loadAll(v);
        }
        for (int i = 0; i < NUM_LFOS; i++)
        {
            processor.lfos[i]->loadAll(v);
        }
        for (int i = 0; i < NUM_FX; i++)
        {
            processor.fx[i]->loadAll(v);
        }
    }
    
}

void ElectroAudioProcessorEditor::updateRandomValueLabel(float value)
{
    randomValueLabel.setText(String(value, 3), dontSendNotification);
}



