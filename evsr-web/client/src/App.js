import {
  NotificationContainer,
  NotificationManager,
} from 'react-notifications';
import React, { Component } from 'react';
import { Route, BrowserRouter as Router } from 'react-router-dom';
import { globalStyles, media } from './styles/styles';
import socket, { socket_url } from './socket.js';
import styled, { ThemeProvider } from 'styled-components';

import Evaluation from './components/Evaluation';
import Navigation from './components/Navigation';
import Search from './components/Search';
import theme from './styles/theme';

//CSS
import '../node_modules/bulma/css/bulma.css';
import '../node_modules/bulma-divider/dist/bulma-divider.min.css';
import '../node_modules/bulma-pageloader/dist/bulma-pageloader.min.css';
import 'react-notifications/lib/notifications.css';

const Page = styled.div`
  background-color: "#e6e6e6";
  display: grid;
  grid-template-rows: 50px 1fr;
  grid-template-columns:1fr ;
  grid-template-areas: 
  "nav"
  "content";
}
`;

const Main = styled.main`
  grid-area: content;
  align-self: center;
  justify-self: center;
  min-width: 960px;
  max-width: 960px;
  min-height: calc(100vh - 50px);
  padding-top: 50px;
  ${media.desktop`
    padding: 0 20px 0 20px;
    min-width: 70%;
    max-width: 70%;
  `};
`;

class App extends Component {
  componentDidMount() {
    socket.on('connect', data => {
      NotificationManager.success(
        `Connected to ${socket_url}`,
        'Connected',
        3000,
      );
    });
    socket.on('connect_error', data => {
      NotificationManager.warning(
        'Lost connection to the server',
        'Warning',
        3000,
      );
    });
  }

  render() {
    return (
      <Router>
        <ThemeProvider theme={theme}>
          <Page>
            <NotificationContainer />
            <Navigation />
            <Main>
              <Route exact path="/" component={Search} />
              <Route path="/evaluation" component={Evaluation} />
            </Main>
          </Page>
        </ThemeProvider>
      </Router>
    );
  }
}

export default App;
