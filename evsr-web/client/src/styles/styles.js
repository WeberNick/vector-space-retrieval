import styled, { css, injectGlobal } from 'styled-components';

import { darken } from 'polished';
import theme from './theme';

// Defines media tags for styled-components
const sizes = {
  giant: 1170,
  desktop: 992,
  tablet: 768,
  phone: 376,
};

// iterate through the sizes and create a media template
export const media = Object.keys(sizes).reduce((accumulator, label) => {
  // use em in breakpoints to work properly cross-browser and support users
  // changing their browsers font-size: https://zellwk.com/blog/media-query-units/
  const emSize = sizes[label] / 16;
  accumulator[label] = (...args) => css`
    @media (max-width: ${emSize}em) {
      ${css(...args)};
    }
  `;
  return accumulator;
}, {});

const gloablStyles = injectGlobal`
  html {
    box-sizing: border-box;
    background: #fff;
    color: #666
  }
  *,
  *::before,
  *::after {
  box-sizing: inherit;
  }
  body {
    margin: 0;
    height:100vh;
  }

  ::selection{
    color: #fff;
    background-color: ${theme.primary};
  }

  a {
    color: ${theme.primary};
    text-decoration: none;
    cursor: pointer;
    &:hover {
      color: ${props => darken(0.1, theme.primary)} !important;
    }
  }


  li p {
    margin: 0;
  }

  img {
    border-radius: 5px;
    box-shadow: 0 14px 25px rgba(0, 0, 0, 0.16);
  }

  pre {
    box-shadow: 0 14px 25px rgba(0, 0, 0, 0.16);
    border-radius: 5px;
  }

  input[type=number]:invalid {
    background-color: ${theme.warning}
    color: #fff
  }

  .radio input{
    margin-right: 5px;
  }

  .shadow-bottom {
    box-shadow: 0 14px 25px rgba(0, 0, 0, 0.16);
  }

  .shadow-bottom-hover{
    transition: box-shadow .5s;
    &:hover{
      box-shadow: 0 14px 25px rgba(0, 0, 0, 0.16);
    }
  }

  .notification-message{
    .message {
      background-color: transparent;
    }
  }

  .is-divider-vertical{
    @media (max-width: 992px) {
      display: none;
    }
  }


  .line-legend{
    li {
      display: inline;
      margin-right: 10px;
      span {
        vertical-align: middle;
        margin-left: 5px;
      }
    }
    .line-legend-icon{
      min-width: 20px;
      min-height: 20px;
      display: inline-block;
    }
  }
 

  .bar-legend{
    li {
      display: inline;
      margin-right: 10px;
      span {
        vertical-align: middle;
        margin-left: 5px;
      }
    }
  

    .bar-legend-icon{
      min-width: 20px;
      min-height: 20px;
      display: inline-block;
    }
  }

  

  
`;

export const Grid = styled.div`
  display: grid;
  grid-template-columns: 1fr; //repeat(auto-fit, minmax(320px, 1fr));
  grid-gap: 1.5rem;
`;
