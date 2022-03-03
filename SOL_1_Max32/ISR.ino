/*
INTERRUPT ROUTINES
*/
void FlowCounter_TapWater_Flow(){
  FlowCount_TapWater_Flow++;
}

void FlowCounter_AckTank_HeatEx_Flow(){
  FlowCount_AckTank_HeatEx_Flow++;
}

void FlowCounter_SolHeatEx_Flow(){
  FlowCount_SolHeatEx_Flow++;
}

uint32_t tmrCallback(uint32_t currentTime) {
  currentFlowCount_TapWater_Flow = FlowCount_TapWater_Flow;
  FlowCount_TapWater_Flow = 0;
  currentFlowCount_AckTank_HeatEx_Flow = FlowCount_AckTank_HeatEx_Flow;
  FlowCount_AckTank_HeatEx_Flow = 0;
  currentFlowCount_SolHeatEx_Flow = FlowCount_SolHeatEx_Flow;
  FlowCount_SolHeatEx_Flow = 0;
  return (currentTime + CORE_TICK_RATE*1000);
}


void __USER_ISR serISR_0() {
//    digitalWrite(13, HIGH);
    if (getIntFlag(_UART1_TX_IRQ)) {
        if (tx_0.available()) {
            U1TXREG = tx_0.read();
            tx_0_Running = true;
        } else {
            clearIntEnable(_UART1_TX_IRQ);
            tx_0_Running = false;
        }
        clearIntFlag(_UART1_TX_IRQ);
    }
    if (getIntFlag(_UART1_RX_IRQ)) {
        rx_0.write(U1RXREG);
        clearIntFlag(_UART1_RX_IRQ);
    }
    if (getIntFlag(_UART1_ERR_IRQ)) {
        clearIntFlag(_UART1_ERR_IRQ);
    }
//    digitalWrite(13, LOW);
}

void __USER_ISR serISR_1() {
//    digitalWrite(13, HIGH);
    if (getIntFlag(_UART4_TX_IRQ)) {
        if (tx_1.available()) {
            U4TXREG = tx_1.read();
            tx_1_Running = true;
        } else {
            clearIntEnable(_UART4_TX_IRQ);
            tx_1_Running = false;
        }
        clearIntFlag(_UART4_TX_IRQ);
    }
    if (getIntFlag(_UART4_RX_IRQ)) {
        rx_1.write(U4RXREG);
        clearIntFlag(_UART4_RX_IRQ);
    }
    if (getIntFlag(_UART4_ERR_IRQ)) {
        clearIntFlag(_UART4_ERR_IRQ);
    }
//    digitalWrite(13, LOW);
}

void __USER_ISR serISR_2() {
//    digitalWrite(13, HIGH);
    if (getIntFlag(_UART2_TX_IRQ)) {
        if (tx_2.available()) {
            U2TXREG = tx_2.read();
            tx_2_Running = true;
        } else {
            clearIntEnable(_UART2_TX_IRQ);
            tx_2_Running = false;
        }
        clearIntFlag(_UART2_TX_IRQ);
    }
    if (getIntFlag(_UART2_RX_IRQ)) {
        rx_2.write(U2RXREG);
        clearIntFlag(_UART2_RX_IRQ);
    }
    if (getIntFlag(_UART2_ERR_IRQ)) {
        clearIntFlag(_UART2_ERR_IRQ);
    }
//    digitalWrite(13, LOW);
}

void __USER_ISR serISR_3() {
//    digitalWrite(13, HIGH);
    if (getIntFlag(_UART5_TX_IRQ)) {
        if (tx_3.available()) {
            U5TXREG = tx_3.read();
            tx_3_Running = true;
        } else {
            clearIntEnable(_UART5_TX_IRQ);
            tx_3_Running = false;
        }
        clearIntFlag(_UART5_TX_IRQ);
    }
    if (getIntFlag(_UART5_RX_IRQ)) {
        rx_3.write(U5RXREG);
        clearIntFlag(_UART5_RX_IRQ);
    }
    if (getIntFlag(_UART5_ERR_IRQ)) {
        clearIntFlag(_UART5_ERR_IRQ);
    }
//    digitalWrite(13, LOW);
}

void kick_0() {
    if (!tx_0_Running) {
        tx_0_Running = true;
        IFS2bits.U1BTXIF = 1;
        setIntEnable(_UART1_TX_IRQ);
    }    
}
void kick_1() {
    if (!tx_1_Running) {
        tx_1_Running = true;
        IFS2bits.U4TXIF = 1;
        setIntEnable(_UART4_TX_IRQ);
    }    
}
void kick_2() {
    if (!tx_2_Running) {
        tx_2_Running = true;
        IFS2bits.U2BTXIF = 1;
        setIntEnable(_UART2_TX_IRQ);
    }    
}
void kick_3() {
    if (!tx_3_Running) {
        tx_3_Running = true;
        IFS2bits.U5TXIF = 1;
        setIntEnable(_UART5_TX_IRQ);
    }    
}

void configureInterrupts(){
  attachCoreTimerService(tmrCallback);

  rx_0.flush();
  rx_1.flush();
  rx_2.flush();
  rx_3.flush();
  tx_0.flush();
  tx_1.flush();
  tx_2.flush();
  tx_3.flush();
  tx_0.onWrite(kick_0);
  tx_1.onWrite(kick_1);
  tx_2.onWrite(kick_2);
  tx_3.onWrite(kick_3);

  setIntVector(_UART_1_VECTOR, serISR_0); //Serial
  setIntVector(_UART_4_VECTOR, serISR_1); //Serial1
  setIntVector(_UART_2_VECTOR, serISR_2); //Serial2
  setIntVector(_UART_5_VECTOR, serISR_3); //Serial3
  setIntPriority(_UART1_RX_IRQ, 4, 0);
  setIntPriority(_UART4_RX_IRQ, 4, 1);
  setIntPriority(_UART2_RX_IRQ, 4, 1);
  setIntPriority(_UART5_RX_IRQ, 4, 1);
  setIntPriority(_UART1_TX_IRQ, 3, 0);
  setIntPriority(_UART4_TX_IRQ, 4, 1);
  setIntPriority(_UART2_TX_IRQ, 4, 1);
  setIntPriority(_UART5_TX_IRQ, 4, 1);
  clearIntFlag(_UART1_RX_IRQ);
  clearIntFlag(_UART4_RX_IRQ);
  clearIntFlag(_UART2_RX_IRQ);
  clearIntFlag(_UART5_RX_IRQ);
  clearIntFlag(_UART1_TX_IRQ);
  clearIntFlag(_UART4_TX_IRQ);
  clearIntFlag(_UART2_TX_IRQ);
  clearIntFlag(_UART5_TX_IRQ);
  attachInterrupt(digitalPinToInterrupt(FL_VV_IN), FlowCounter_TapWater_Flow, FALLING);
  attachInterrupt(digitalPinToInterrupt(FL_PVV), FlowCounter_AckTank_HeatEx_Flow, FALLING);
  attachInterrupt(digitalPinToInterrupt(FL_SOL), FlowCounter_SolHeatEx_Flow, FALLING);
  setIntEnable(_UART1_TX_IRQ);
  setIntEnable(_UART4_TX_IRQ);
  setIntEnable(_UART2_TX_IRQ);
  setIntEnable(_UART5_TX_IRQ);
  setIntEnable(_UART1_RX_IRQ);
  setIntEnable(_UART4_RX_IRQ);
  setIntEnable(_UART2_RX_IRQ);
  setIntEnable(_UART5_RX_IRQ);
}



