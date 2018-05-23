import React, { Component } from 'react';

import { Link } from 'react-router-dom';
import { media } from '../styles/styles';
import styled from 'styled-components';

const TopNavigation = styled.nav`
  grid-area: nav;
  height: 50px;
  display: flex;
  position: fixed;
  width: 100%;
  z-index: 1;
  background-color: #fff;
  ${media.desktop`
    padding: 0;
  `}
}
`;

const Container = styled.div`
  align-items: stretch;
  display: flex;
  width: 100%;
  margin: 0 auto;
  max-width: 960px;
`;

const Brand = styled.div`
  align-items: stretch;
  display: flex;
  flex-shrink: 0;
  min-height: 3.25rem;
`;

const NavItem = styled.div`
  padding: 0.5rem 0.75rem;
  h1 {
    a {
      color: #000;
    }
  }
`;

const NavMenu = styled.div`
  flex-grow: 1;
  flex-shrink: 0;
`;

const NavStart = styled.div`
  justify-content: flex-start;
  margin-right: auto;
  align-items: stretch;
  display: flex;
  min-height: 3.25rem;
}
`;

class Navigation extends Component {
  render() {
    return (
      <TopNavigation className="shadow-bottom">
        <Container>
          <Brand>
            <NavItem>
              <h1>
                <Link to={'/app'}>EVSR</Link>
              </h1>
            </NavItem>
          </Brand>
          <NavMenu>
            <NavStart>
              <NavItem>
                <Link to={'/app/evaluation'}>Evaluation</Link>
              </NavItem>
              <NavItem>
                <Link to="/presentation" target="_self">
                  Presentation
                </Link>
              </NavItem>
            </NavStart>
          </NavMenu>
        </Container>
      </TopNavigation>
    );
  }
}

export default Navigation;
