// Import React
import React from 'react';

// Import Spectacle Core tags
import {
  BlockQuote,
  CodePane,
  Cite,
  Deck,
  Heading,
  ListItem,
  List,
  Quote,
  Slide,
  Text,
  Table,
  TableRow,
  TableHeaderItem,
  TableItem,
  Layout,
  TableHeader,
  TableBody,
  Image,
} from 'spectacle';

// Import theme
import createTheme from 'spectacle/lib/themes/default';

import '../prism-atom-dark.css';

// Require CSS
require('normalize.css');

const theme = createTheme(
  {
    primary: 'white',
    secondary: '#1F2022',
    tertiary: '#03A9FC',
    quarternary: '#CECECE',
  },
  {
    primary: 'Montserrat',
    secondary: 'Helvetica',
  },
);

const images = {
  city: require('../assets/city.jpg'),
  kat: require('../assets/kat.png'),
  logo: require('../assets/formidable-logo.svg'),
  markdown: require('../assets/markdown.png'),
};

export default class Presentation extends React.Component {
  render() {
    return (
      <Deck
        transition={['zoom', 'slide']}
        transitionDuration={500}
        theme={theme}
      >
        <Slide transition={['slide']} bgColor="primary">
          <Heading size={1} fit caps lineHeight={1} textColor="secondary">
            Efficient Vector Space Retrieval
          </Heading>
          <Text margin="10px 0 0" textColor="tertiary" size={1} fit bold>
            Making Vector Space Retrieval Lightning Fast ⚡
          </Text>
        </Slide>
        <Slide transition={['slide']} bgColor="tertiary">
          <Heading size={6} textColor="primary" caps>
            Agenda
          </Heading>
          <List textColor="primary">
            <ListItem>Our Idea</ListItem>
            <ListItem>Why this ?</ListItem>
            <ListItem>Implementation</ListItem>
            <ListItem>Results</ListItem>
          </List>
        </Slide>
        <Slide transition={['slide']}>
          <Heading
            size={6}
            textColor="tertiary"
            caps
            style={{ marginBottom: '30px' }}
          >
            Building the indexes
          </Heading>
          <CodePane
            lang="cpp"
            source={require('raw-loader!../assets/index_manager_init.example')}
            theme="external"
            margin="20px auto"
            overflow="overflow"
          />
          <Text margin="10px 0 0" textColor="secondary" lineHeight={0.5}>
            Builds every index we need during retrieval
          </Text>
        </Slide>
        <Slide
          transition={['slide']}
          bgColor="primary"
          notes="Hard to find cities without any pizza"
        >
          <Heading size={6} caps style={{ marginBottom: '30px' }}>
            Retrieval Results
          </Heading>
          <Layout>
            <Table>
              <TableHeader>
                <TableRow>
                  <TableHeaderItem />
                  <TableHeaderItem>2011</TableHeaderItem>
                  <TableHeaderItem>2013</TableHeaderItem>
                  <TableHeaderItem>2015</TableHeaderItem>
                </TableRow>
              </TableHeader>
              <TableBody>
                <TableRow>
                  <TableItem>None</TableItem>
                  <TableItem>61.8%</TableItem>
                  <TableItem>39.6%</TableItem>
                  <TableItem>35.0%</TableItem>
                </TableRow>
                <TableRow>
                  <TableItem>Pineapple</TableItem>
                  <TableItem>28.3%</TableItem>
                  <TableItem>54.5%</TableItem>
                  <TableItem>61.5%</TableItem>
                </TableRow>
                <TableRow>
                  <TableItem>Pepperoni</TableItem>
                  <TableItem />
                  <TableItem>50.2%</TableItem>
                  <TableItem>77.2%</TableItem>
                </TableRow>
                <TableRow>
                  <TableItem>Olives</TableItem>
                  <TableItem />
                  <TableItem>24.9%</TableItem>
                  <TableItem>55.9%</TableItem>
                </TableRow>
              </TableBody>
            </Table>
          </Layout>
        </Slide>
        <Slide>
          <Image src={images.kat.replace('/', '')} margin="0px auto 40px" />
        </Slide>
      </Deck>
    );
  }
}
